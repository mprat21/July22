#include "bandatac.h"
#include "banlogicimpl.h"
using namespace BANLogic;
namespace BANLogic {

bool BANLogic::BANLogicImpl::getMatchingPostulates(LPT::Statement *stmt, LPT::PostulatePtrList &pl)
{    BanStatementList *goal1 = dynamic_cast<BanStatementList *>(stmt);
     bool foundPostulates = false;
      if (goal1 != nullptr) {
          BanPostulates *p;
          BanStatementList *s = nullptr;
          // just be safe, clear pl
          pl.clear();
          for (int i=0; i<post.count(); ++i) {
              p = dynamic_cast<BanPostulates *>(post.at(i)->getCopy());
              // std::cout << post.at(i)->getString().toStdString() << " evaluate Postulate " << p->getString().toStdString() << std::endl;
              // cout<<endl;
              s = dynamic_cast<BanStatementList *>(p->getGoal());
              if (s->unify(*goal1)) {
                  foundPostulates = true;
                  pl.append(p);
                  //std::cout << "added Postulate " << p->getString().toStdString() << " to matching postulates" << std::endl;
              }
          }
      }
      return foundPostulates;
}

bool BANLogic::BANLogicImpl::getAllInstantiatedPostulates(LPT::Postulate *p, LPT::PostulatePtrList &ipl)
{
    //mycomment->append(QString("Logic BAN \n"));
    BanDataList *de;
    bool instPostulates = false;
    QList<BanDComponent*> dList;
    BanPostulates *copy;
    BanPostulates *instPostulate = dynamic_cast<BanPostulates *>(p->getCopy());
    BanSComponent *X,*Y;
    BanStatementList *goalBan;
    const BanStatementList *s;
    ComponentPtrListS instantiations;
    BanRuleId id= instPostulate->getRuleID();
    switch (instPostulate->getRuleID())
    {
    case BanRuleId::B1://P believes X,  P believes Y -> P believes (X,Y)
    case BanRuleId::F1://P believes fresh (X) -> P believes fresh (X,Y)
    case BanRuleId::F2://P believes fresh (Y) -> P believes fresh (X,Y)
    case BanRuleId::NV://P believes fresh(X), P believes Q said X -> P believes Q believes X
    { if (!ipl.contains(instPostulate))
        {
            instPostulates=true;
            ipl.append(instPostulate);
        }

        break;}
    case BanRuleId::B2://P believes (X,Y) -> P believes X
    case BanRuleId::B3://P believes (X,Y) -> P believes Y
    case BanRuleId::S2://P sees <X>Y -> P sees X
    case BanRuleId::S1://P sees(X,Y) -> P sees X
    case BanRuleId::S6://P sees <X>Y -> P sees Y
    case BanRuleId::S7://P sees (X,Y) -> P sees Y
    case BanRuleId::S4://P believes P has K+ as a public key, P sees {X}K+ -> P sees X
    case BanRuleId::J:   //P believes Q controls X , P believes Q believes X -> P believes X
    case BanRuleId::B4:  //P believes Q believes (X,Y) -> P believes Q believes X
    case BanRuleId::B5:  //P believes Q believes (X,Y) -> P believes Q believes Y
    case BanRuleId::B6:  //P believes Q said (X,Y) -> P believes Q said X
    case BanRuleId::B7:  //P believes Q said (X,Y) -> P believes Q said Y
    case BanRuleId::S3://P believes K is shared with Q, P sees {X}K -> P sees X
    case BanRuleId::S5://P believes Q has K+ as a public key, P sees {X}K- -> P sees X
    case BanRuleId::I1://P believes key K is shared with Q , P sees {X}K ->  P believes Q said X
    case BanRuleId::I2:  //P believes Q has public key K+ , P sees {X}K- ->  P believes Q said X
    case BanRuleId::I3:  //P believes Y is shared secret between Q and P , P sees <X>Y ->  P believes Q said X
    case BanRuleId::B8:  //P believes X, P believes Q believes (X,Y) -> P believes (X,Q)
    { goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
        for(int k=0; k<sList.count(); k++)
        {
            switch(sList.value(k)->getStype())
            {
            case BanSComponentType::bData:
            {
                de = dynamic_cast<BanDataList *>(sList.value(k));
                //dList=de->getTempStorage();
                //for(int l=0;l<dList.count(); l++)
                // mycomment->append(QString(dList.at(l)->getString()));
                break;
            }

            }
        }

        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(i));
            // mycomment->append(X->getString());

            // s->findData(*X,sList, false,false);
        }
        if (!ipl.contains(instPostulate)) {
            ipl.append(instPostulate);
            instPostulates=true;
        }

        break;
    }
    default:
        throw UnrecognisedPostulateException("Unrecognised Postulate Exception in BAN::Logic::getAllInstantiatedPostulates");
    }
    return instPostulates;

}
QTextBrowser *BANLogicImpl::getMycomment() const
{
    return mycomment;
}

void BANLogicImpl::setMycomment(QTextBrowser *value)
{
    mycomment = value;
}

BANLogicImpl::BANLogicImpl(QTextBrowser *comment)
{
    mycomment=comment;
    // show();
    BanPostulates *p;
    LPT::StatementPtrList preList2;
    QList<BanStatementList*> preList;
    BanDAtom *aP=new BanDAtom(Principal,"");
    BanDAtom *aQ=new BanDAtom(Principal,"");
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
        BanStatementList *temp=new BanStatementList({
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::I1,goal,preList2);
        post.append(p);
        preList2.clear();
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
        BanStatementList *temp=new BanStatementList({
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::I2,goal,preList2);
        post.append(p);
        preList2.clear();

    }

    //for shared secret
    {
        //2.2 P believes Y is shared secret between Q and P , P sees <X>Y ->  P believes Q said X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({aP,aQ,Y,new BanDOperator(BanDOperatorType::ShareSecret)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *temp=new BanStatementList({
                                                        new BanDataList({aQ}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::said)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::I3,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    //3: Nonce verification rule
    {
        //3.1: P believes fresh(X), P believes Q said X -> P believes Q believes X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *temp1=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::said)
                                                     });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::NV,goal,preList2);
        post.append(p);
        preList2.clear();

    }

    //4: Jurisdiction rule
    {
        //4.1: P believes Q controls X , P believes Q believes X -> P believes X
        BanStatementList *temp1=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::controls)
                                                     });
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::J,goal,preList2);
        post.append(p);
        preList2.clear();

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

        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::S1,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    {
        //5.2: P sees <X>Y -> P sees X
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });

        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::S2,goal,preList2);
        post.append(p);
        preList2.clear();

    }

    {
        //5.3: P believes K is shared with Q, P sees {X}K -> P sees X
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
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });

        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::S3,goal,preList2);
        post.append(p);
        preList2.clear();

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
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::S4,goal,preList2);
        post.append(p);
        preList2.clear();

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

        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::S5,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    {
        //5.6: P sees <X>Y -> P sees Y
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::Encryption)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({Y}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });

        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::S6,goal,preList2);
        post.append(p);
        preList2.clear();

    }
    {
        //5.7: P sees (X,Y) -> P sees Y
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y,new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({Y}),
                                                        new BanSOperator(BanSOperatorType::sees)
                                                    });

        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::S7,goal,preList2);
        post.append(p);
        preList2.clear();

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
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::F1,goal,preList2);
        post.append(p);
        preList2.clear();
    }
    {
        //6.2 : P believes fresh (Y) -> P believes fresh (X,Y)
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({Y,new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates),new BanDOperator(BanDOperatorType::FreshData)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::F2,goal,preList2);
        post.append(p);
        preList2.clear();
    }


    //newly added

    //7.1 : P believes X,  P believes Y -> P believes (X,Y)
    {
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({Y}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);
        p=new BanPostulates(BanRuleId::B1,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    //7.2 : P believes (X,Y) -> P believes X
    {
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B2,goal,preList2);
        post.append(p);
        preList2.clear();
    }
    //7.3 : P believes (X,Y) -> P believes Y
    {
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({Y}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B3,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    //7.4 : P believes Q believes (X,Y) -> P believes Q believes X
    {
        BanStatementList *temp1=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B4,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    //7.5 : P believes Q believes (X,Y) -> P believes Q believes Y
    {

        BanStatementList *temp1=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({Y}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B5,goal,preList2);
        post.append(p);
        preList2.clear();
    }


    //7.6 : P believes Q said (X,Y) -> P believes Q said X
    {
        BanStatementList *temp1=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                         new BanSOperator(BanSOperatorType::said)
                                                     });
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X}),
                                                         new BanSOperator(BanSOperatorType::said)
                                                     });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B6,goal,preList2);
        post.append(p);
        preList2.clear();
    }
    //7.7 : P believes Q said (X,Y) -> P believes Q said Y
    {BanStatementList *temp1=new BanStatementList({
                                                      new BanDataList({aQ}),
                                                      new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                      new BanSOperator(BanSOperatorType::said)
                                                  });

        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp1,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({Y}),
                                                         new BanSOperator(BanSOperatorType::said)
                                                     });
        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        p=new BanPostulates(BanRuleId::B7,goal,preList2);
        post.append(p);
        preList2.clear();
    }

    //7.8 : P believes X, P believes Q believes (X,Y) -> P believes (X,Q)
    {
        BanStatementList *pre1=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *temp2=new BanStatementList({
                                                         new BanDataList({aQ}),
                                                         new BanDataList({X,Y, new BanDOperator(BanDOperatorType::concates)}),
                                                         new BanSOperator(BanSOperatorType::believes)
                                                     });
        BanStatementList *pre2=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        temp2,
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });

        BanStatementList *goal=new BanStatementList({
                                                        new BanDataList({aP}),
                                                        new BanDataList({X,aQ, new BanDOperator(BanDOperatorType::concates)}),
                                                        new BanSOperator(BanSOperatorType::believes)
                                                    });
        preList2.append(pre1);
        preList2.append(pre2);

        p=new BanPostulates(BanRuleId::B8,goal,preList2);
        post.append(p);
        preList2.clear();
    }

}
}
