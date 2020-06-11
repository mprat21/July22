#include "bandatac.h"
#include "banlogicimpl.h"
using namespace BANLogic;

BANLogicImpl::BANLogicImpl()
{
}

void BANLogicImpl::show()
{

    BanDataList *dcomp,*d, *newData1,*newData2, *newData3,*newData4, *pm1, *pm2;
    pm1=new BanDataList({
                            new BanDAtom(Principal,"A"),
                            new BanDataC(),
                            new BanDOperator(BanDOperatorType::concates),
                            new BanDAtom(SymKey,"Kab"),
                            new BanDOperator(BanDOperatorType::Encryption)
                        });

    pm2=new BanDataList({
                            new BanDAtom(Principal,"A"),
                            new BanDAtom(Principal,"B"),
                            new BanDAtom(Nonce,"Na"),
                            new BanDOperator(BanDOperatorType::concates),
                            new BanDAtom(PrivKey,"Ka+"),
                            new BanDOperator(BanDOperatorType::Encryption),
                            new BanDOperator(BanDOperatorType::concates),
                            new BanDAtom(SymKey,"Kab"),
                            new BanDOperator(BanDOperatorType::Encryption)
                        });

    d=new BanDataList({
                          new BanDAtom(Principal,""), new BanDAtom(Principal,""),
                          new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                          new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                          new BanDAtom(Nonce,""), new BanDOperator(BanDOperatorType::concates),
                          new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                          new BanDOperator(BanDOperatorType::Encryption)
                      });

    dcomp=new BanDataList({
                              new BanDAtom(Principal,"A"), new BanDAtom(Principal,"B"),
                              new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                              new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                              new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                              new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                              new BanDOperator(BanDOperatorType::Encryption)
                          });


    newData1=new BanDataList({
                                 new BanDAtom(Principal,""),
                                 new BanDataC(),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,""),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });
    newData2=new BanDataList({
                                 new BanDAtom(Principal,"B"),
                                 new BanDAtom(Principal,"A"),
                                 new BanDAtom(SymKey,"Na"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kab"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });

cout<<endl;

    newData3=new BanDataList({
                                 new BanDAtom(Principal,""),
                                 new BanDataC(),
                                 new BanDAtom(Principal,""),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kap"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });

    newData4=new BanDataList({
                                 new BanDAtom(Principal,"A"),
                                 new BanDAtom(Principal,"B"),
                                 new BanDAtom(Nonce,"Na"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(PrivKey,"Ka+"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(Principal,"L"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kap"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });
    cout<<endl<<"Before unification newdata3"<<endl;
    newData3->print();
    cout<<endl<<"Before unification newdata4"<<endl;

    newData4->print();

//    if(newData3->match(*newData4))
//    {
//        cout<<endl;
//        newData3->print();
//        cout<<"    matches    ";
//        newData4->print();
//    }
//    else
//    {
//        cout<<endl;
//        newData3->print(); cout<<"  does not  matches    ";newData4->print();
//    }

    cout<<endl<<"After unification:  "<<endl;
    if(newData3->unify(*newData4))
    {
        cout<<endl;
        newData3->printRPN();
        //newData3->print();

        //newData3->print(); cout<<"    unifies    ";newData4->print();
    }
    else
    {
        cout<<endl;
        newData3->print(); cout<<"  does not  unify    ";newData4->print();
    }

    //-------------------------------------------------------------------------------------------------
/*
    QList<BanDComponent*> datalis;
    QList<BanSComponent*> stList;
    datalis.append(new BanDAtom(Principal,"A"));
    stList.append(new BanDataList(datalis));
    stList.append(dcomp);
    stList.append(new banSOperator(BanSOperatorType::possess));
    BanStatementList *sscomp=new BanStatementList(stList);
    cout<<"Printing the Statement 1 in RPN notation:"<<endl;
    sscomp->printRPN();
    cout<<endl;
    cout<<"Printing the Statement 1 in Infix notation:"<<endl;
    sscomp->print();
    cout<<endl;

    QList<BanDComponent*> datalist,d2;
    QList<BanSComponent*> stListt;
    datalist.append(new BanDAtom(Principal,"B"));
    d2.append(new BanDAtom(Principal,"C"));
    stListt.append(new BanDataList(datalist));
    stListt.append(stList);
    stListt.append(new banSOperator(BanSOperatorType::believe));
    BanStatementList *sscompt=new BanStatementList(stListt);
    cout<<"Printing the Statement inside Statement in RPN notation:"<<endl;
    sscompt->printRPN();
    cout<<endl;
    cout<<"Printing the Statement inside Statement in Infix notation:"<<endl;
    sscompt->print();
    cout<<endl;

    QList<BanDComponent*> datalist2,d21;
    QList<BanSComponent*> stListt1;
    datalist2.append(new BanDAtom(Principal,"D"));
    d21.append(new BanDAtom(Principal,"F"));
    stListt1.append(new BanDataList(datalist2));
    stListt1.append(stListt);
    stListt1.append(new banSOperator(BanSOperatorType::believe));
    BanStatementList *sscompt1=new BanStatementList(stListt1);
    cout<<"Printing the multiple Statements inside  statment in RPN notation:"<<endl;
    sscompt1->printRPN();
    cout<<endl;
    cout<<"Printing the multiple Statements inside  statment in Infix notation:"<<endl;
    sscompt1->print();
    cout<<endl;
*/
}


