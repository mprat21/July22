#include "bandatac.h"
#include "banlogicimpl.h"
using namespace BANLogic;
namespace BANLogic {

BANLogic::BANLogicImpl::BANLogicImpl()
{
    BanPostulates *p;
    QList<BanStatementList*> preList;
    BanDAtom *aP=new BanDAtom(Principal,"");
    BanDAtom *aQ=new BanDAtom(Principal,"");
    BanDAtom *aR=new BanDAtom(Principal,"");
    BanDAtom *aS=new BanDAtom(Principal,"");  // trusted Server
    BanDAtom *K=new BanDAtom(SymKey,"");
    BanDAtom *aKPriv=new BanDAtom(PrivKey,"");
    BanDAtom *aKPub=new BanDAtom(PubKey,"");
    BanDAtom *aF=new BanDAtom(Function,"");
    BanDAtom *aH=new BanDAtom(Hash,"");
    aKPriv->setInverseKey(aKPub);
    aKPub->setInverseKey(aKPriv);
    BanDataC *X=new BanDataC();
    BanDataC *Y=new BanDataC();

    //1: Syntax and informal semantics
    {
        //1.1: P believes X:
        BanStatementList *St1=new BanStatementList({
                                                       new BanDataList({aP}),
                                                       new BanDataList({X}),
                                                       new BanSOperator(BanSOperatorType::believes)
                                                   });

        //1.2: P sees X:
        BanStatementList *St2=new BanStatementList({
                                                       new BanDataList({aP}),
                                                       new BanDataList({X}),
                                                       new BanSOperator(BanSOperatorType::sees)
                                                   });
        //1.3: P said X:
        BanStatementList *St3=new BanStatementList({
                                                       new BanDataList({aP}),
                                                       new BanDataList({X}),
                                                       new BanSOperator(BanSOperatorType::said)
                                                   });
        //1.4: P controls X:
        BanStatementList *St4=new BanStatementList({
                                                       new BanDataList({aP}),
                                                       new BanDataList({X}),
                                                       new BanSOperator(BanSOperatorType::controls)
                                                   });

        //1.5: fresh X:
        BanStatementList *St5=new BanStatementList({
                                                       new BanDataList({X,new BanDOperator(BanDOperatorType::FreshData)}),
                                                   });

        //1.6: P & Q shares key K:
        BanStatementList *St6=new BanStatementList({
                                                       new BanDataList({aP,aQ,K,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                   });

        //1.7: P has K as a public key:
        BanStatementList *St7=new BanStatementList({
                                                       new BanDataList({aP,aKPub,new BanDOperator(BanDOperatorType::HasKey)}),
                                                   });

        //1.8: P and Q knows secret X:
        BanStatementList *St8=new BanStatementList({
                                                       new BanDataList({aP,aQ,X,new BanDOperator(BanDOperatorType::ShareSecret)}),
                                                   });

        //1.9: X is encrypted by Key K:
        BanStatementList *St9=new BanStatementList({
                                                       new BanDataList({X,K,new BanDOperator(BanDOperatorType::Encryption)}),
                                                   });

        //1.10: X combined with the formula Y or <X>Y;
        BanStatementList *St10=new BanStatementList({
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::SecretPassword)}),
                                                    });
    }

    //2: Logical Postulates
    //interpretation of messages
    {
        //2.1 P believes key K is shared with Q , P sees {X}K ->  P believes Q said X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aP,aQ,K,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,K,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("I1",goal,preList);
        BANPostulates.append(p);
    }

    //for public key
    {
        //2.2 P believes Q has public key K+ , P sees {X}K- ->  P believes Q said X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ,aKPub,new BanDOperator(BanDOperatorType::HasKey)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,aKPriv,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("I2",goal,preList);
        BANPostulates.append(p);
    }

    //for shared secret
    {
        //2.2 P believes Y is shared secret between Q and P , P sees <X>Y ->  P believes Q said X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ,aP,Y,new BanDOperator(BanDOperatorType::ShareSecret)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::SecretPassword)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("I3",goal,preList);
        BANPostulates.append(p);
    }

    //3: Nonce verification rule
    {
        //3.1: P believes fresh(X), P believes Q said X -> P believes Q believes X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("NV",goal,preList);
        BANPostulates.append(p);
    }

    //4: Jurisdiction rule
    {
        //4.1: P believes Q controls X , P believes Q believes X -> P believes X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::controls),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("J",goal,preList);
        BANPostulates.append(p);
    }

    //5: If a principal sees a formula, then he also sees its components, provided he knows the necessary keys:
    {
        //5.1: P sees (X,Y) -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        preList.append(pre1);
        p=new BanPostulates("S1",goal,preList);
        BANPostulates.append(p);
    }

    {
        //5.2: P sees <X>Y -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::SecretPassword)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        preList.append(pre1);
        p=new BanPostulates("S2",goal,preList);
        BANPostulates.append(p);
    }

    {
        //5.3: P believes K is shared with Q, P sees {X}K -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ,aP,K,new BanDOperator(BanDOperatorType::ShareKey)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,K,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("S3",goal,preList);
        BANPostulates.append(p);
    }

    {
        //5.4: P believes P has K+ as a public key, P sees {X}K+ -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aP,aKPub,new BanDOperator(BanDOperatorType::HasKey)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,aKPub,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("S4",goal,preList);
        BANPostulates.append(p);
    }

    {
        //5.5: P believes Q has K+ as a public key, P sees {X}K- -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aQ,aKPub,new BanDOperator(BanDOperatorType::HasKey)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,aKPriv,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        preList.append(pre1);
        preList.append(pre2);
        p=new BanPostulates("S5",goal,preList);
        BANPostulates.append(p);
    }

    // 6 :If one part of a formula is fresh, then the entire formula must also be fresh:
    {
        //6.1 : P believes fresh (X) -> P believes fresh (X,Y)
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates),new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList.append(pre1);
        p=new BanPostulates("S5",goal,preList);
        BANPostulates.append(p);

        pre1->print();
        cout<<endl;
        cout<<endl;
        goal->print();
        cout<<endl;
        p->print();
        p->printRPN();
    }
}

void BANLogicImpl::show()
{
    /*
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
    }*/

}

}
