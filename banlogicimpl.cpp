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
                                 new BanDAtom(Principal,"B"),
                                 new BanDataC(),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,""),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });
    newData2=new BanDataList({
                                 new BanDAtom(Principal,""),
                                 new BanDAtom(Principal,"A"),
                                 new BanDAtom(SymKey,"Na"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kab"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });

    cout<<endl<<"Before unification newdata3"<<endl;
    newData1->print();
    cout<<endl<<endl<<"Before unification newdata4"<<endl;
    newData2->print();
    cout<<endl;


    if(newData1->unify(*newData2))
    {
        cout<<endl<<"After unification newdata 1 and newdata 2 are:  "<<endl;
        newData1->print(); cout<<"    matches and unifies    ";newData1->print();
    }
    else
    {
        cout<<endl;
        newData1->print(); cout<<"  does not match and unify    ";newData2->print();
    }

    cout<<endl<<endl<<endl;

    newData3=new BanDataList({
                                 new BanDAtom(Principal,""),
                                 new BanDataC(),
                                 new BanDAtom(Principal,""),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kab"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });

    newData4=new BanDataList({
                                 new BanDAtom(Principal,"A"),
                                 new BanDAtom(Principal,"C"),
                                 new BanDAtom(Principal,"B"),
                                 new BanDAtom(Nonce,"Na"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(PrivKey,"Ka+"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(Principal,"S"),
                                 new BanDOperator(BanDOperatorType::concates),
                                 new BanDAtom(SymKey,"Kab"),
                                 new BanDOperator(BanDOperatorType::Encryption),
                             });
    cout<<endl<<"Before unification newdata3"<<endl;
    newData3->print();
    cout<<endl<<endl<<"Before unification newdata4"<<endl;
    newData4->print();
    cout<<endl;


    if(newData3->unify(*newData4))
    {
        cout<<endl<<"After unification newdata 3 and new data 4 are:  "<<endl;
        newData3->print(); cout<<"    matches and unifies    ";newData4->print();
    }
    else
    {
        cout<<endl;
        newData3->print(); cout<<"  does not match and unify    ";newData4->print();
    }

    //-------------------------------------------------------------------------------------------------
  /*
    BanStatementList *stList1;
    BanStatementList *stList2,*stList3;
    BanStatementList* stList4;
    stList1= new BanStatementList({
                                      new BanDataList({new BanDAtom(Principal,"A")}),
                                      new BanDataList({
                                          new BanDAtom(Principal,"P"), new BanDAtom(Principal,"Q"),
                                          new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                                          new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                                          new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                                          new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                                          new BanDOperator(BanDOperatorType::Encryption)
                                      }),
                                      new banSOperator(BanSOperatorType::possess)
                                  });
    cout<<endl<<endl<<"Printing the Statement 1 in RPN notation:"<<endl;
    stList1->printRPN();
    cout<<endl;
    cout<<"Printing the Statement 1 in Infix notation:"<<endl;
    stList1->print();
    cout<<endl<<endl;
    stList2 = new BanStatementList({
                                       new BanDataList({new BanDAtom(Principal,"B")}),
                                       new BanDataList({new BanDAtom(Principal,"A")}),
                                       new BanDataList({
                                           new BanDAtom(Principal,"P"), new BanDAtom(Principal,"Q"),
                                           new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                                           new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                                           new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                                           new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                                           new BanDOperator(BanDOperatorType::Encryption)
                                       }),
                                       new banSOperator(BanSOperatorType::possess),
                                       new banSOperator(BanSOperatorType::believe)
                                   });

    cout<<"Printing the Statement 2 in RPN notation:"<<endl;
    stList2->printRPN();
    cout<<endl;
    cout<<"Printing the Statement 2 in Infix notation:"<<endl;
    stList2->print();
    cout<<endl;
    cout<<endl<<endl;

    stList3=new BanStatementList({
                                     new BanDataList({new BanDAtom(Principal,"D")}),
                                     stList1,
                                     new banSOperator(BanSOperatorType::told)
                                 });
    cout<<"Printing the Statement 3 in RPN notation:"<<endl;
    stList3->printRPN();
    cout<<endl;
    cout<<"Printing the Statement 3 in Infix notation:"<<endl;
    stList3->print();
    cout<<endl;
    cout<<endl<<endl;

    stList4 =new BanStatementList({
                                      new BanDataList({new BanDAtom(Principal,"F")}),
                                      new BanDataList({new BanDAtom(Principal,"B")}),
                                      stList2,
                                      new banSOperator(BanSOperatorType::believe),
                                      new banSOperator(BanSOperatorType::conveyed)
                                  });
    cout<<"Printing the Statement 4 in RPN notation:"<<endl;
    stList4->printRPN();
    cout<<endl;
    cout<<"Printing the Statement 4 in Infix notation:"<<endl;
    stList4->print();
    cout<<endl;
    cout<<endl<<endl;

    BanStatementList *stList5=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"F")}),
                                                     new BanDataList({new BanDAtom(Principal,"B")}),
                                                     stList1,
                                                     new banSOperator(BanSOperatorType::believe),
                                                     new banSOperator(BanSOperatorType::conveyed)
                                                   });
    cout<<"Printing the multiple Statements 5 in RPN notation:"<<endl;
    stList5->printRPN();
    cout<<endl;
    cout<<"Printing the multiple Statements 5 in Infix notation:"<<endl;
    stList5->print();
    cout<<endl;
    cout<<endl<<endl;
    */

         cout<<endl<<endl<<endl;
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
        cout<<endl<<endl<<endl;
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
        cout<<endl<<endl<<endl;
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
}


