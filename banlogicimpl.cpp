#include "banlogicimpl.h"
using namespace BANLogic;

BANLogicImpl::BANLogicImpl()
{
}

void BANLogicImpl::show()
{
    BanDataList *dcomp,*d;
    dcomp=new BanDataList({new BanDAtom(Principal,"A"), new BanDAtom(Principal,"B"),
                           new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                           new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                           new BanDAtom(Nonce,"Nb"), new BanDOperator(BanDOperatorType::concates),
                           new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                           new BanDOperator(BanDOperatorType::Encryption)
                          });
    d=new BanDataList({new BanDAtom(Principal,""), new BanDAtom(Principal,""),
                       new BanDAtom(Nonce,"Na"), new BanDOperator(BanDOperatorType::concates),
                       new BanDAtom(SymKey,"Kab"), new BanDOperator(BanDOperatorType::Encryption),
                       new BanDAtom(Nonce,""), new BanDOperator(BanDOperatorType::concates),
                       new BanDOperator(BanDOperatorType::concates), new BanDAtom(SymKey,"Kbc"),
                       new BanDOperator(BanDOperatorType::Encryption)
                      });
    cout<<endl;
    cout<<"Printing dcomp data list in RPN notation:"<<endl;
    dcomp->printRPN();
    cout<<endl;
    cout<<"Printing dcomp data list in Infix notation:"<<endl;
    dcomp->print();
    cout<<endl<<endl<<"Printing d data list in RPN notation:"<<endl;
    d->printRPN();
    cout<<endl<<endl<<"Printing d data list in Infix notation:"<<endl;
    d->print();
    cout<<endl<<endl<<"Check if the list dcomp matches with d list:"<<endl;
    if(dcomp->match(*d))
    {
    //    dcomp->print();cout<<" == "; d->print(); cout<<endl;
    }
    else
    {
   //     dcomp->print();cout<<"  != ";d->print();cout<<endl;
    }
    cout<<endl;
  //  cout<<"display which free elements can be unified and unify it:"<<endl;
    cout<<endl;
    if(dcomp->unify(*d))
    {
       // cout<<"After unification print dcomp in rpn and infix:"<<endl;
       // dcomp->printRPN(); dcomp->print();
      //  cout<<endl<<endl<<"After unification print d in rpn and infix:"<<endl;
        //d->printRPN();d->print();
    }
    cout<<endl;

    //match and unify for type data
    BanDataList *newData,*newData2;
    newData=new BanDataList({new BanDataList(),
                             new BanDataList(),new BanDataList(),new BanDataList()});
    newData2=new BanDataList({new BanDAtom(Principal,"A"),
                              new BanDAtom(SymKey,"Kbc"),
                              new BanDOperator(BanDOperatorType::Encryption)
                             });
    cout<<"checking newData1"<<endl;
    newData->getID();
    cout<<"checking newData2"<<endl;
    newData2->print();
    //cout<<newData->match(*newData2);








    //-------------------------------------------------------------------------------------------------
    /*
    QList<BanDComponent*> datalis;
    QList<BanSComponent*> stList;
    datalis.append(new BanDAtom(Principal,"A"));
    stList.append(new BanDataList(datalis));
    stList.append(new BanDataList(compList));
    stList.append(new banSOperator(BanSOperatorType::possess));
    BanStatementList *sscomp=new BanStatementList(stList);
   // cout<<"Printing the Statement 1 in RPN notation:"<<endl;
   // sscomp->printRPN();
    cout<<endl;
  //  cout<<"Printing the Statement 1 in Infix notation:"<<endl;
   // sscomp->print();
    cout<<endl;

    QList<BanDComponent*> datalist,d2;
    QList<BanSComponent*> stListt;
    datalist.append(new BanDAtom(Principal,"B"));
    d2.append(new BanDAtom(Principal,"C"));
    stListt.append(new BanDataList(datalist));
    stListt.append(stList);
    stListt.append(new banSOperator(BanSOperatorType::believe));
    BanStatementList *sscompt=new BanStatementList(stListt);
   // cout<<"Printing the Statement inside Statement in RPN notation:"<<endl;
   // sscompt->printRPN();
    cout<<endl;
   // cout<<"Printing the Statement inside Statement in Infix notation:"<<endl;
   // sscompt->print();
    cout<<endl;

    QList<BanDComponent*> datalist2,d21;
    QList<BanSComponent*> stListt1;
    datalist2.append(new BanDAtom(Principal,"D"));
    d21.append(new BanDAtom(Principal,"F"));
    stListt1.append(new BanDataList(datalist2));
    stListt1.append(stListt);
    stListt1.append(new banSOperator(BanSOperatorType::believe));
    BanStatementList *sscompt1=new BanStatementList(stListt1);
   // cout<<"Printing the multiple Statements inside  statment in RPN notation:"<<endl;
   // sscompt1->printRPN();
    cout<<endl;
   // cout<<"Printing the multiple Statements inside  statment in Infix notation:"<<endl;
   // sscompt1->print();
    cout<<endl;

*/
}


