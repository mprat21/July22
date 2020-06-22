#include "bandatac.h"
#include "banlogicimpl.h"
using namespace BANLogic;

BANLogicImpl::BANLogicImpl()
{
}

void BANLogicImpl::show()
{
    try {
        BanDataList *dcomp,*d, *newData1,*newData2, *newData3,*newData4, *pm1, *pm2, *mylist1, *mylist2, *newData5, *newData6;


        cout<<endl<<"--------------Printing simple lists in RPN and Infix notation----------"<<endl;

        mylist1=new BanDataList({
                                    new BanDAtom(Principal,"A"),
                                    new BanDAtom(Principal,"B"),
                                    new BanDOperator(BanDOperatorType::concates),
                                    new BanDAtom(SymKey,"Kab"),
                                    new BanDOperator(BanDOperatorType::Encryption)
                                });

        cout<<endl<<"String representation of Simple List 1 in RPN notation"<<endl;
        mylist1->printRPN();
        cout<<endl<<"String representation of Simple List 1 in Infix notation"<<endl;
        mylist1->print();
        cout<<endl<<endl;

        mylist2=new BanDataList({
                                    new BanDAtom(Principal,"A"),
                                    new BanDAtom(Principal,"B"),
                                    new BanDAtom(Principal,"C"),
                                    new BanDOperator(BanDOperatorType::concates),
                                    new BanDOperator(BanDOperatorType::concates),
                                    new BanDAtom(SymKey,"Kab"),
                                    new BanDOperator(BanDOperatorType::Encryption)
                                });

        cout<<endl<<"String representation of Simple List 2 in RPN notation"<<endl;
        mylist2->printRPN();
        cout<<endl<<"String representation of Simple List 2 in Infix notation"<<endl;
        mylist2->print();
        cout<<endl<<endl<<endl;


        cout<<endl<<"--------------Printing complex lists in RPN and Infix notation----------"<<endl;
        dcomp=new BanDataList({
                                  new BanDAtom(Principal,"A"), new BanDAtom(Principal,"B"),
                                  new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                                  new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                                  new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                                  new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                                  new BanDOperator(BanDOperatorType::Encryption)
                              });

        cout<<endl<<"String representation of dataList 1 in RPN notation"<<endl;
        dcomp->printRPN();
        cout<<endl<<"String representation of dataList 1 in Infix notation"<<endl;
        dcomp->print();
        cout<<endl<<endl<<endl;

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
        cout<<endl<<"String representation of dataList 2 in RPN notation"<<endl;
        pm2->printRPN();
        cout<<endl<<"String representation of dataList 2 in Infix notation"<<endl;
        pm2->print();
        cout<<endl<<endl;

        cout<<endl<<"---------------Printing lists with free elements in RPN and Infix notation----------"<<endl;
        pm1=new BanDataList({
                                new BanDAtom(Principal,""),
                                new BanDataC(),
                                new BanDOperator(BanDOperatorType::concates),
                                new BanDAtom(SymKey,""),
                                new BanDOperator(BanDOperatorType::Encryption)
                            });

        cout<<endl<<"String representation of free dataList 1 in RPN notation"<<endl;
        pm1->printRPN();
        cout<<endl<<"String representation of free dataList 1 in Infix notation"<<endl;
        pm1->print();
        cout<<endl<<endl;

        d=new BanDataList({
                              new BanDAtom(Principal,""), new BanDAtom(Principal,""),
                              new BanDAtom(Nonce,""), new BanDOperator(BanDOperatorType::concates),
                              new BanDAtom(SymKey,""), new BanDOperator(BanDOperatorType::Encryption),
                              new BanDAtom(Nonce,""), new BanDOperator(BanDOperatorType::concates),
                              new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,""),
                              new BanDOperator(BanDOperatorType::Encryption)
                          });
        cout<<endl<<"String representation of free dataList 2 in RPN notation"<<endl;
        d->printRPN();
        cout<<endl<<"String representation of free dataList 2 in Infix notation"<<endl;
        d->print();
        cout<<endl<<endl;


        cout<<endl<<"---------------Checking if the free data/atoms from list1 matches and can be unified with instantiated ones from List2----------"<<endl;

        cout<<endl<< "1: Check the below lists if they match and unify------------------"<<endl;
        newData1=new BanDataList({
                                     new BanDAtom(Principal,""),
                                     new BanDataC(),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(SymKey,""),
                                     new BanDOperator(BanDOperatorType::Encryption),
                                 });
        newData2=new BanDataList({
                                     new BanDAtom(Principal,"A"),
                                     new BanDAtom(Principal,"B"),
                                     new BanDAtom(SymKey,"Na"),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(SymKey,"Kab"),
                                     new BanDOperator(BanDOperatorType::Encryption),
                                 });

        cout<<endl<<"Before unification newdata1"<<endl;
        newData1->print();
        cout<<endl<<endl<<"Before unification newdata2"<<endl;
        newData2->print();
        cout<<endl;
        cout<<endl<<"Below elements would be unified:";
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
        cout<<endl<< "2: Check the below lists if they match and unify---------------"<<endl;
        newData3=new BanDataList({
                                     new BanDAtom(Principal,""),
                                     new BanDataC(),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(Principal,""),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(SymKey,""),
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
        cout<<endl<<"Below elements would be unified:";
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
        cout<<endl;
        cout<<endl;

        cout<<endl<< "3: Check the below lists if they match and unify-------------"<<endl;
        newData5=new BanDataList({
                                     new BanDAtom(Principal,""),
                                     new BanDataC(),
                                     new BanDAtom(Principal,""),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(Principal,""),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(SymKey,""),
                                     new BanDOperator(BanDOperatorType::Encryption),
                                 });

        newData6=new BanDataList({
                                     new BanDAtom(Principal,"A"),
                                     new BanDAtom(Principal,"C"),
                                     new BanDAtom(Principal,"B"),
                                     new BanDAtom(Nonce,"Na"),
                                     new BanDAtom(Nonce,"Nb"),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(PrivKey,"Ka+"),
                                     new BanDOperator(BanDOperatorType::Encryption),
                                     new BanDAtom(Principal,"P"),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(Principal,"S"),
                                     new BanDOperator(BanDOperatorType::concates),
                                     new BanDAtom(SymKey,"Ka"),
                                     new BanDOperator(BanDOperatorType::Encryption),
                                 });
        cout<<endl<<"Before unification newdata5"<<endl;
        newData5->print();
        cout<<endl<<endl<<"Before unification newdata6"<<endl;
        newData6->print();
        cout<<endl;
        if(newData5->unify(*newData6))
        {
            cout<<endl<<"After unification newdata 5 and new data 6 are:  "<<endl;
            newData5->print(); cout<<"    matches and unifies    ";newData6->print();
        }
        else
        {
            cout<<endl;
            newData5->print(); cout<<"  does not match and unify    ";newData6->print();
        }
        cout<<endl;
        cout<<endl;


        //-------------------------------------------------------------------------------------------------

        cout<<endl<<endl<<"------------Printing the Statements in RPN and Infix notation--------------"<<endl;

        BanStatementList *stList1;
        BanStatementList *stList2,*stList3;
        BanStatementList* stList4;
        stList1= new BanStatementList({
                                          new BanDataList({new BanDAtom(Principal,"A")}),
                                          new BanDataList({
                                              new BanDAtom(Principal,"P"), new BanDAtom(Principal,"B"),
                                              new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                                              new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                                              new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                                              new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                                              new BanDOperator(BanDOperatorType::Encryption)
                                          }),
                                          new BanSOperator(BanSOperatorType::possess)
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
                                           new BanSOperator(BanSOperatorType::possess),
                                           new BanSOperator(BanSOperatorType::believe)
                                       });

        cout<<endl<<endl<<"Printing the Statement 2 in RPN notation:"<<endl;
        stList2->printRPN();
        cout<<endl;
        cout<<"Printing the Statement 2 in Infix notation:"<<endl;
        stList2->print();
        cout<<endl;
        cout<<endl<<endl;

        stList3=new BanStatementList({
                                         new BanDataList({new BanDAtom(Principal,"D")}),
                                         stList1,
                                         new BanSOperator(BanSOperatorType::told)
                                     });
        cout<<endl<<endl<<"Printing the Statement 3 in RPN notation:"<<endl;
        stList3->printRPN();
        cout<<endl;
        cout<<"Printing the Statement 3 in Infix notation:"<<endl;
        stList3->print();
        cout<<endl;
        cout<<endl<<endl;

        stList4 =new BanStatementList({
                                          new BanDataList({new BanDAtom(Principal,"C")}),
                                          new BanDataList({new BanDAtom(Principal,"S")}),
                                          stList2,
                                          new BanSOperator(BanSOperatorType::believe),
                                          new BanSOperator(BanSOperatorType::conveyed)
                                      });
        cout<<endl<<endl<<"Printing the Statement 4 in RPN notation:"<<endl;
        stList4->printRPN();
        cout<<endl;
        cout<<"Printing the Statement 4 in Infix notation:"<<endl;
        stList4->print();
        cout<<endl;
        cout<<endl<<endl;

        BanStatementList *stList5=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"F")}),
                                                         new BanDataList({new BanDAtom(Principal,"B")}),
                                                         stList1,
                                                         new BanSOperator(BanSOperatorType::believe),
                                                         new BanSOperator(BanSOperatorType::conveyed)
                                                       });
        cout<<"Printing the multiple Statements 5 in RPN notation:"<<endl;
        stList5->printRPN();
        cout<<endl;
        cout<<"Printing the multiple Statements 5 in Infix notation:"<<endl;
        stList5->print();
        cout<<endl;
        cout<<endl<<endl;

        cout<<"-------------Next task to do: matching and unification for free elements in a statement with the instantiated ones-----------------------"<<endl;

        BanStatementList *stList6=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"")}),
                                                         new BanDataList({new BanDataC()}),
                                                         new BanSOperator(BanSOperatorType::told)
                                                       });
        cout<<endl<<"Printing the free data in Statement 6 in RPN notation:"<<endl;
        stList6->printRPN();
        cout<<endl;
        cout<<"Printing the free data in Statement 6 in Infix notation:"<<endl;
        stList6->print();
        cout<<endl;
        cout<<endl<<endl;

        BanStatementList *stList7=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"A")}),
                                                         new BanDataList({
                                                             new BanDAtom(Principal,"P"), new BanDAtom(Principal,"Q"),
                                                             new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                                                             new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                                                             new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                                                             new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                                                             new BanDOperator(BanDOperatorType::Encryption)
                                                         }),
                                                         new BanSOperator(BanSOperatorType::told)
                                                       });
        cout<<"Printing the data : Statement 7 in RPN notation:"<<endl;
        stList7->printRPN();
        cout<<endl;
        cout<<"Printing the data : Statement 7 in Infix notation:"<<endl;
        stList7->print();
        cout<<endl;
        cout<<endl<<endl;
        if(stList6->unify(*stList7))
        {
            cout<<endl;
            stList6->print();
            cout<<endl<<"matches n unifies"<<endl;
            stList7->print();

        }
        else
        {
            cout<<endl;
            stList6->print();
            cout<<endl<<"does not match/unify"<<endl;
            stList7->print();
        }
        cout<<endl<<endl;

//---------------------------------------------------------------------------------------------------------


        BanStatementList *stList8=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"")}),
                                                         stList6,
                                                         new BanSOperator(BanSOperatorType::believe)
                                                       });
        cout<<endl<<"Printing the Statement in Statement 8 in RPN notation:"<<endl;
        stList8->printRPN();
        cout<<endl;
        cout<<"Printing the Statement in Statement 8 in Infix notation:"<<endl;
        stList8->print();
        cout<<endl;
        cout<<endl<<endl;

        BanStatementList *stList9=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"B")}),
                                                         stList7,
                                                         new BanSOperator(BanSOperatorType::believe)
                                                       });
        cout<<"Printing the Statement in: Statement 9 in RPN notation:"<<endl;
        stList9->printRPN();
        cout<<endl;
        cout<<"Printing the Statement in : Statement 9 in Infix notation:"<<endl;
        stList9->print();
        cout<<endl;
        cout<<endl<<endl;
        if(stList8->unify(*stList9))
        {
            cout<<endl;
            stList8->print();
            cout<<endl<<"matches n unifies"<<endl;
            stList9->print();

        }
        else
        {
            cout<<endl;
            stList8->print();
            cout<<endl<<"does not match/unify"<<endl;
            stList9->print();
        }
        cout<<endl<<endl;

//--------------------------------------------------------------------------------------------------------------------------------------


        BanStatementList *stList10=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"")}),
                                                         stList1,
                                                         new BanSOperator(BanSOperatorType::conveyed)
                                                       });
        cout<<endl<<"Printing the free Statement in Statement 10 in RPN notation:"<<endl;
        stList10->printRPN();
        cout<<endl;
        cout<<"Printing the free Statement in Statement 10 in Infix notation:"<<endl;
        stList10->print();
        cout<<endl;
        cout<<endl<<endl;

        BanStatementList *stList11=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"S")}),
                                                         stList1,
                                                         new BanSOperator(BanSOperatorType::conveyed)
                                                       });
        cout<<"Printing the Statement : Statement 11 in RPN notation:"<<endl;
        stList11->printRPN();
        cout<<endl;
        cout<<"Printing the Statement : Statement 11 in Infix notation:"<<endl;
        stList11->print();
        cout<<endl;
        cout<<endl<<endl;
        if(stList10->unify(*stList11))
        {
            cout<<endl;
            stList10->print();
            cout<<endl<<"matches n unifies"<<endl;
            stList11->print();

        }
        else
        {
            cout<<endl;
            stList10->print();
            cout<<endl<<"does not match/unify"<<endl;
            stList11->print();
        }
//-------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------default statement: any statement-------------------------------------

        BanStatementList *stList12=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"")}),
                                                         new BanStatementList(),
                                                         new BanSOperator(BanSOperatorType::believe)
                                                       });
        cout<<endl<<endl<<endl<<"Printing the any Statement : free Statement 12 in RPN notation:"<<endl;
        stList12->printRPN();
        cout<<endl;
        cout<<"Printing the any Statement in free Statement 12 in Infix notation:"<<endl;
        stList12->print();
        cout<<endl;
        cout<<endl<<endl;

        BanStatementList *stList13=new BanStatementList({ new BanDataList({new BanDAtom(Principal,"C")}),
                                                          stList1,
                                                         new BanSOperator(BanSOperatorType::believe)
                                                       });
        cout<<"Printing the Statement : Statement 13 in RPN notation:"<<endl;
        stList13->printRPN();
        cout<<endl;
        cout<<"Printing the Statement : Statement 13 in Infix notation:"<<endl;
        stList13->print();
        cout<<endl;
        cout<<endl<<endl;
        if(stList12->unify(*stList13))
        {
            cout<<endl;
            stList12->print();
            cout<<endl<<"matches/unify"<<endl;
            stList13->print();
        }
        else
        {
            cout<<endl;
            stList12->print();
            cout<<endl<<"does not match/unify"<<endl;
            stList13->print();
        }
    } catch (BanException &e)
    {
        e.what("Handled exception");
    }

}


