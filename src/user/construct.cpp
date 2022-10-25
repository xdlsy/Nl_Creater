#include "construct.h"
#include "assert.h"
#define log 0
void init_net(MyNetlist* global_nl, Parser* global_parser){
    int din_begin = global_parser->GetDinRange()[1];
    int din_end = global_parser->GetDinRange()[0];
    int dout_begin = global_parser->GetDoutRange()[1];
    int dout_end = global_parser->GetDoutRange()[0];
    int sel_begin = global_parser->GetSelRange()[1];
    int sel_end = global_parser->GetSelRange()[0];
    // 将SEL pins加入电路
    std::cout<<"sel_begin:"<<sel_begin<<" sel_end:"<<sel_end<<
    std::endl<<"din_begin:"<<din_begin<<" din_end:"<<din_end<<
    std::endl<<"dout_begin:"<<dout_begin<<" dout_end:"<<dout_end<<
    std::endl;
    for(int i = sel_begin; i < sel_end; i++){
        global_nl->CreateModelSelPin(i);
    }

    //恒1 pin和恒0 pin
    MyPin* zero = new MyPin(new Pin(false));
    MyPin* one = new MyPin(new Pin(false));
    zero->SetName("const_0");
    zero->Tie0();
    one->SetName("const_1");
    one->Tie1();
    global_nl->AddInPin(0, zero);
    global_nl->AddInPin(1, one);

    // 将Din pins及其取反加入电路，如果某个输入的反没有用到，就不加入
    MATRIX& data_mat = global_parser->DataMat();
    std::set<int> used_din;
    for(auto& i:data_mat){
        for(auto& j: i){
            used_din.insert(j);
        }
    }
    for(int i = din_begin; i < din_end; i++){
        global_nl->CreateModelInPin(i, used_din.find(i*2+3)!=used_din.end());
    }

    // 将Dout pins加入电路
    for(int i = dout_begin; i < dout_end; i++){
        global_nl->CreateModelOutPin(i);
    }
}
void construct_mux_from_matrix(MyNetlist* global_nl, Parser* global_parser){
    // 第一步，向netlist中添加din pins，sel pins和dout pins
    init_net(global_nl, global_parser);

    // 第二步，为每一个输出端口构造一个电路
    int ndout = global_nl->GetId2OutPin().size();
    int ndin = global_nl->GetId2InPin().size();
    std::cout<<"ndout="<<ndout<<" ndin="<<ndin<<std::endl;
    for(int i = 0; i < ndout; i++){
        construct_baseline_sub_circuit(i, global_nl, global_parser);
        std::cout<<"done "<<i<<"th out"<<std::endl;
    }

    // 第三步，综合优化，缩减时延和面积
    // optimization();

}

void construct_baseline_sub_circuit(int outpin_idx, MyNetlist* global_nl, Parser* global_parser){
    MATRIX& addr_mat = global_parser->AddrMat();
    MATRIX& data_mat = global_parser->DataMat();
    int nsel = addr_mat[0].size();
    std::map<int, std::vector<std::vector<int>>> dval_to_svals; // 所有使得输出为d[23]（举例）的sel向量
    for(int i = 0; i < addr_mat.size(); i++){
        int dval = data_mat[i][outpin_idx]; //data_mat的第i行第outpin_idx列
        dval_to_svals[dval].push_back(addr_mat[i]);
    }
    
    int _default = data_mat[addr_mat.size()][outpin_idx];
    build_mux_tree(dval_to_svals, global_nl, global_nl->GetId2OutPin()[outpin_idx], _default);
    int i = 0;
}

// void build_mux_tree()
// 记问题为（sels，Dins）
// 记sel_i=1时的dout集合为Din1, sel_i=0时的dout集合为Din0，
// 从剩余的sels中找到sel_i，使得其Din1与Din0的交集为空集，
// 可认为sel_i控制一个mux，sel_i=0时，mux的输出来自Din0，sel_i=1时，mux的输出来自Din1
// 将sel_i从sels中删除，然后分别求解子问题（sels - sel_i, Din0）和（sels - sel_i, Din1），直到|sels|=0或者|Dins|=1
typedef std::map<int, std::vector<std::vector<int>>> d2svals;
void build_mux_tree(d2svals& dval_to_svals, MyNetlist* global_nl, MyPin* out, int _default){
    std::set<int> sels;
    std::map<int, MyPin*>& id2Sel = global_nl->GetId2Sel();
    std::map<int, MyPin*>& id2InPin = global_nl->GetId2InPin();
    for(auto i : global_nl->GetId2Sel()){
        sels.insert(i.first);
    }
    std::set<int> initial_dins;
    for(auto i : dval_to_svals){
        initial_dins.insert(i.first);
    }

    std::map<MyPin*, int> first_answer;
    std::map<MyPin*, std::set<int>> usedsels;
    // 用dfs的方式求解会造成电路一条腿长一条腿短，所以用bfs的方式求解
    std::queue<MyPin*> queue_fa; //
    std::queue<std::set<int>> queue_dins;
    std::queue<std::set<int>> queue_usedsel;
    std::queue<std::set<int>> queue_remainsel;
    std::queue<d2svals> queue_d2svals;
    queue_dins.push(initial_dins); // 子电路包含的dins
    queue_fa.push(out); // 子电路的流向
    queue_usedsel.push(std::set<int>());
    queue_remainsel.push(sels);
    queue_d2svals.push(dval_to_svals);

    while(queue_dins.size()){

        // 取子问题
        std::set<int> _dins = queue_dins.front(); queue_dins.pop();
        MyPin* fa = queue_fa.front(); queue_fa.pop();
        std::set<int> used_sels = queue_usedsel.front();queue_usedsel.pop();
        std::set<int> remain_sels = queue_remainsel.front(); queue_remainsel.pop();
        d2svals _dval_to_svals = queue_d2svals.front(); queue_d2svals.pop();

        //打印信息
        if(log){
            std::cout<<std::endl<<"sub question:"<<std::endl;
            std::cout<<"fa name:"<<fa->Name()<<std::endl;
            std::cout<<"dins:";
            for(int d:_dins){
                std::cout<<d<<" ";
            }std::cout<<std::endl;
            std::cout<<"remain sels: ";
            for(int ss: remain_sels){
                std::cout<<ss<<" ";
            }std::cout<<std::endl;
        }
        // 如果子问题中只剩一个din，且，则直接将该din与mux相连
        if(_dins.size() == 1 && remain_sels.size() == 0){ 
            int din_idx = *(_dins.begin());
                        if(log)
                        std::cout<<"connect din_"<<din_idx<<" "<<id2InPin[din_idx]<<" to fa="<<fa->Name()<<std::endl;
            global_nl->Disconnect(id2InPin[din_idx], fa);
            //collect information for optimization
            if(din_idx == 0 || din_idx == 1){
                global_nl->const_mux.insert(fa->GetPart());
            }
            global_nl->top_mux.insert(fa->GetPart());
            continue;
        }
        // 如果子问题中没有din，则与default相连
        else if(_dins.size() == 0){
            MyPin* _default_in = id2InPin[_default];
                        if(log)
                        std::cout<<"connect default="<<_default<<" to fa="<<fa->Name()<<std::endl;
            global_nl->Connect(_default_in, fa);
            //collect information for optimization
            if(_default == 0 || _default == 1){
                global_nl->const_mux.insert(fa->GetPart());
            }
            global_nl->top_mux.insert(fa->GetPart());
            continue;
        }
        std::set<int> dins_to_zero, dins_to_one;
        // 找出sel_i, 和它的Din0，Din1       
        int s = find_best_sel(dins_to_zero, dins_to_one, remain_sels, _dins, _dval_to_svals); 
        // 为sel_i创建一个mux并连接
        MyMux* mux = global_nl->CreateMux(s);
                    if(log)
                    std::cout<<"connect mux->GetOutPin()="<<mux->GetOutPin()->Name()<<" to fa="<<fa->Name()<<std::endl;
        //将mux的输出连接至下一个mux
        global_nl->Connect(mux->GetOutPin(), fa);
                    if(log)
                    std::cout<<"connect sels[s]="<<id2Sel[s]->Name()<<" to mux->GetSelPin()="<<mux->GetSelPin()->Name()<<" s="<<s<<std::endl;
        //将sel与mux相连
        global_nl->Connect(id2Sel[s], mux->GetSelPin());

        // 将Dins0和Dins1这两个子问题入队，mux的两个输入端分别对应两个子电路的流向，也入队。
        queue_fa.push(mux->GetI0Pin());
        queue_fa.push(mux->GetI1Pin());
                if(log){
                    std::cout<<"creat question, dins_to_zero:";
                    for(int d:dins_to_zero)
                        {std::cout<<d<<" ";}
                    std::cout<<" fa:"<<mux->GetI0Pin()->Name()<<std::endl;
                    std::cout<<"creat question, dins_to_one:";
                    for(int d:dins_to_one)
                    {std::cout<<d<<" ";}
                    std::cout<<" fa:"<<mux->GetI1Pin()->Name()<<std::endl;
                }
        queue_dins.push(dins_to_zero);
        queue_dins.push(dins_to_one);
        used_sels.insert(s);
        queue_usedsel.push(used_sels);
        queue_usedsel.push(used_sels);
        remain_sels.erase(s);
        queue_remainsel.push(remain_sels);
        queue_remainsel.push(remain_sels);
        d2svals d2svals_0, d2svals_1;
        for(int d: dins_to_zero){
            for(auto& addr: _dval_to_svals[d]){
                if(addr[s]==0 || addr[s]==-1){
                    d2svals_0[d].push_back(addr);
                }
            }
        }
        for(int d: dins_to_one){
            for(auto& addr: _dval_to_svals[d]){
                if(addr[s]==1 || addr[s]==-1){
                    d2svals_1[d].push_back(addr);
                }
            }
        }
        queue_d2svals.push(d2svals_0);
        queue_d2svals.push(d2svals_1);
    }

}

int find_best_sel(std::set<int>& dins_to_zero, std::set<int>& dins_to_one, std::set<int>& sels, std::set<int>& dins, d2svals& dval_to_svals){
    std::map<int, std::set<int> > zero_dins, one_dins; // 分别为sel_i=0时，输出端口可能出现的值，和sel_i=1时，输出端口可能出现的值
    for(const int& d : dins){
                if(log)
                    std::cout<<"d:"<<d<<std::endl<<"addrs:"<<std::endl;
        for(std::vector<int>& addr : dval_to_svals[d]){
                if(log){
                    for(int j=0;j<addr.size();j++){
                        std::cout<<addr[j];
                    }std::cout<<std::endl;
                }
            for(const int& sel : sels){
                // if( addr[sel] == 0 && one_dins[sel].find(d) == one_dins[sel].end()){
                if( addr[sel] == 0){
                    zero_dins[sel].insert(d);
                }
                // else if( addr[sel] == 1 && zero_dins[sel].find(d) == zero_dins[sel].end()){
                else if( addr[sel] == 1){
                    one_dins[sel].insert(d);

                }
                else if (addr[sel]==-1){
                    // "? not considered";
                    zero_dins[sel].insert(d);
                    one_dins[sel].insert(d);
                }
            }
        }
    }
    double best_score = -1000;
    int best_sel = -1;
    // std::cout<<"sels.size():"<<sels.size()<<std::endl;
    for(int sel : sels){
        int n1 = zero_dins[sel].size();
        int n2 = one_dins[sel].size();
        // std::cout<<"n1:"<<n1<<"n2:"<<n2<<std::endl;
        double n = n1 + n2;
        if(n != 0){
            double score = 0;
            if(n1 != 0){
                score -= (n1/n)*log2(n1/n);
            }
            if(n2 != 0){
                score -= (n2/n)*log2(n2/n);
            }
            if(score > best_score){
                best_score = score;
                best_sel = sel;
            }
            // std::cout<<"n1:"<<n1<<" n2:"<<n2<<" score:"<<score<<" best_score:"<<best_score<<" best_sel:"<<best_sel<<std::endl;
        }
    }
    assert(best_sel != -1);
    dins_to_zero = zero_dins[best_sel];
    dins_to_one = one_dins[best_sel];
    return best_sel;
}

// 判断两个集合是否有相同的元素
bool check_redundancy(std::set<int>& set1, std::set<int>& set2){
    std::vector<int> reps;
    for(int i : set1){
        if(set2.find(i) != set2.end()){
            return false;
        }
    }
    return true;
}
void remove_redundancy(std::set<int>& set1, std::set<int>& set2){
    std::vector<int> reps;
    for(int i : set1){
        if(set2.find(i) != set2.end()){
            set2.erase(i);
            reps.push_back(i);
        }
    }
    for(int i : reps){
        set1.erase(i);
    }
}

template<typename T, typename N> int argmax(N& values, T& index){
        double mval = values[0];
        int arg = 0;
        for(int idx : index){
            if(values[idx] > mval){
                mval = values[idx];
                arg = idx;
            }
        }
        return arg;
}
