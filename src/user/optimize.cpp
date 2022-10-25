#include "optimize.h"

void remove_const_input(MyNetlist* global_nl){
    std::vector<int> mux_remove;
    std::map<int, MyMux*>& id2mux = global_nl->GetId2Mux();
    MyPin* const_zero = global_nl->GetId2InPin()[0];
    MyPin* const_one = global_nl->GetId2InPin()[1];
    for(auto& i: id2mux){
        int id = i.first;
        MyMux* mux = i.second;
        if(mux->GetI0Pin() == const_zero || mux->GetI0Pin() == const_one ||
            mux->GetI1Pin() == const_zero)
        if(mux->GetI0Pin() == const_zero) // sel=0, out=0, sel=1, out=d
        {
            MyPin* sel = mux->GetSelPin();
            MyPin* out = mux->GetOutPin();
            MyPin* din = mux->GetI1Pin();
            if(din == const_one)
            {
                
            }
        }
        else if(mux->GetI0Pin() == const_one) // sel=0, out=1, sel=1, out=d
        {

        }
        else if(mux->GetI1Pin() == const_zero) // sel=0, out=d, sel=1, out=0
        {

        }
        else if(mux->GetI1Pin() == const_one) // sel=0, out=d, sel=1, out=1
        {
            std::cout << "test";
        }
    }
}

void remove_same_input_mux(MyNetlist* global_nl) {
    std::unordered_set<MyPart*> top_mux = global_nl->top_mux;
    std::unordered_set<MyPart*> temp_top_mux;
    while(true) {
        while(!top_mux.empty()) {
            for(std::unordered_set<MyPart*>::iterator ite = top_mux.begin(); ite != top_mux.end(); ++ite) {
                MyMux* mux = dynamic_cast<MyMux*>(*ite);
                MyPin* I0 = mux->GetI0Pin()->GetInPin();
                MyPin* I1 = mux->GetI1Pin()->GetInPin();
                // 如果输入相同
                if (I0 == I1) {
                    // 删除MUX与输入的连线，与输出的连线，与sel的连线，将输入直接与输出直接相连
                    global_nl->Disconnect(I0, mux->GetI0Pin());
                    global_nl->Disconnect(I1, mux->GetI1Pin());
                    global_nl->Disconnect(mux->GetSelPin()->GetInPin(), mux->GetSelPin());
                    std::set<MyPin*>& Outs = mux->GetOutPin()->GetOutPins();
                    for (MyPin* const & out: Outs) {
                        global_nl->Disconnect(mux->GetOutPin(), out);
                        global_nl->Connect(I0, out);
                        temp_top_mux.insert(out->GetPart());
                    }
                    // 删除MUX
                    delete mux;
                }
            }
        }
        if (temp_top_mux.empty())
            break;
        top_mux = temp_top_mux;
        temp_top_mux.clear();
    }
}