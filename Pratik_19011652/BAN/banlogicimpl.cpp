#include "bandatac.h"
#include "banlogicimpl.h"

#include <QMessageBox>
using namespace BANLogic;
namespace BANLogic {

bool BANLogic::BANLogicImpl::getMatchingPostulates(LPT::Statement *stmt, LPT::PostulatePtrList &pl)
{
    BanStatementList *YSt;
    BanDataList *YList;
    BanStatementList *s = dynamic_cast<BanStatementList *>(stmt);
     bool foundPostulates = false;
      if (s != nullptr) {
          BanPostulates *p;
          BanStatementList *go = nullptr;
          // just be safe, clear pl
          pl.clear();
          for (int i=0; i<post.count(); ++i) {
              p = dynamic_cast<BanPostulates *>(post.at(i)->getCopy());
              go = dynamic_cast<BanStatementList *>(p->getGoal());
              if (go->unify(*s)) {
                  foundPostulates = true;
                  pl.append(p);
                  QMessageBox::critical(nullptr, "value p", p->getString());
                  QMessageBox::critical(nullptr, "value preq", p->getPrerequisites().at(0)->getString());

                   // mycomment->append(QString());

                  YSt=dynamic_cast<BanStatementList*>(p->getPrerequisites().at(0));
                  YList=dynamic_cast<BanDataList*>(YSt->getStList().at(1));

                  cout<< p->getPrerequisites().at(0)->getString().toStdString();
              }
          }
      }
      return foundPostulates;
}

bool BANLogic::BANLogicImpl::getAllInstantiatedPostulates(LPT::Postulate *p, LPT::PostulatePtrList &ipl)
{
    QStack<BanDComponent *> otherPart;
    QStack<BanDOperator *> op;
    //mycomment->append(QString("Logic BAN \n"));
    BanDataList *de,*XList,*YList;
    BanDAtom *single;
    BanDComponent *dy;
    bool instPostulates = false;
    QList<BanDComponent*> dList;
    BanPostulates *copy;
    BanPostulates *instPostulate = dynamic_cast<BanPostulates *>(p->getCopy());
    BanSComponent *X,*Y;
    BanStatementList *goalBan, *YSt;
    const BanStatementList *s, *st;
    ComponentPtrListD instantiations, principals;
    BanRuleId id= instPostulate->getRuleID();
    switch (instPostulate->getRuleID())
    {
    case BanRuleId::B1://P believes X,  P believes Y -> P believes (X,Y)
    { if (!ipl.contains(instPostulate))
        {
            instPostulates=true;
            ipl.append(instPostulate);
        }
        BanPostulates *instPostulate1 = dynamic_cast<BanPostulates *>(p);

        YSt=dynamic_cast<BanStatementList*>(instPostulate1->getPrerequisites().at(0));
        YList=dynamic_cast<BanDataList*>(YSt->getStList().at(1));

        cout<< instPostulate->getPrerequisites().at(0)->getString().toStdString();

        break;
    }
    case BanRuleId::F1://P believes fresh (X) -> P believes fresh (X,Y)
    { if (!ipl.contains(instPostulate))
        {
            instPostulates=true;
            ipl.append(instPostulate);
        }
        break;
    }

    case BanRuleId::F2://P believes fresh (Y) -> P believes fresh (X,Y)
    {
        goalBan = instPostulate->getBanGoal();
        de=dynamic_cast<BanDataList *>(goalBan->getStList().at(1));
        single= dynamic_cast<BanDAtom*>(de->getDataList().value(1));
        copy = dynamic_cast<BanPostulates*>(instPostulate->getCopy());
        otherPart.append(single);
        // instantiate Y component in prerequisite with found instantiations
        YSt=dynamic_cast<BanStatementList*>(copy->getPrerequisites().at(0));
        YList=dynamic_cast<BanDataList*>(YSt->getStList().at(1));
        YList->instantiateAnyData(otherPart, op,0);
        if (!ipl.contains(copy)) {
            ipl.append(copy);
            instPostulates=true;
        }
        cout<< copy->getPrerequisites().at(0)->getString().toStdString();
        while(!otherPart.isEmpty())
        {
           otherPart.pop();
        }
        break;
    }
    case BanRuleId::NV://P believes fresh(X), P believes Q said X -> P believes Q believes X
    {
        goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
            st = dynamic_cast<BanStatementList *>(sList.value(1));
            XList= dynamic_cast<BanDataList*>(st->getStList().value(0));
        principals.append(XList->getDataList().at(0));

        if (!ipl.contains(instPostulate))
        {
            instPostulates=true;
            ipl.append(instPostulate);
        }
        break;
    }
    case BanRuleId::B4:  //P believes Q believes (X,Y) -> P believes Q believes X
    case BanRuleId::B6:  //P believes Q said (X,Y) -> P believes Q said X
    { goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
        for(int k=0; k<sList.count(); k++)
        {
            if(sList.value(k)->getStype()== BanSComponentType::bStatement)
            {
                st = dynamic_cast<BanStatementList *>(sList.value(k));
                XList= dynamic_cast<BanDataList*>(st->getStList().value(1));
                break;
            }
        }
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(i));
            s->findData(XList,instantiations, false,false);
        }
        for (int i=0; i<instantiations.count(); ++i)
        {
            copy = dynamic_cast<BanPostulates*>(instPostulate->getCopy());
            otherPart.append(instantiations.at(i));
            // instantiate Y component in prerequisite with found instantiations
            YSt=dynamic_cast<BanStatementList*>(copy->getPrerequisites().at(0));
            YList=dynamic_cast<BanDataList*>(dynamic_cast<BanStatementList*>(YSt->getStList().at(1))->getStList().at(1));
            YList->instantiateAnyData(otherPart, op,1);
            // Y->instantiate(instantiations.at(i));
            if (!ipl.contains(copy)) {
                ipl.append(copy);
                instPostulates=true;
            }
            cout<< copy->getPrerequisites().at(0)->getString().toStdString();
            while(!otherPart.isEmpty())
            {
               otherPart.pop();
            }
        }
        break;
    }
    case BanRuleId::B2://P believes (X,Y) -> P believes X
    case BanRuleId::S2://P sees <X>Y -> P sees X
    case BanRuleId::S1://P sees(X,Y) -> P sees X
    { goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
        for(int k=1; k<sList.count(); k++)
        {
            if(sList.value(k)->getStype()== BanSComponentType::bData)
            {
                de = dynamic_cast<BanDataList *>(sList.value(k));
                XList= dynamic_cast<BanDataList*>(de->getDataList().value(1));
                break;
            }
        }
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(i));
            s->findData(XList,instantiations, false,false);
        }
        for (int i=0; i<instantiations.count(); ++i)
        {
            copy = dynamic_cast<BanPostulates*>(instPostulate->getCopy());
            otherPart.append(instantiations.at(i));
            // instantiate Y component in prerequisite with found instantiations
            YSt=dynamic_cast<BanStatementList*>(copy->getPrerequisites().at(0));
            YList=dynamic_cast<BanDataList*>(YSt->getStList().at(1));
            YList->instantiateAnyData(otherPart, op,1);
            if (!ipl.contains(copy)) {
                ipl.append(copy);
                instPostulates=true;

            }
            cout<< copy->getPrerequisites().at(0)->getString().toStdString();
            while(!otherPart.isEmpty())
            {
               otherPart.pop();
            }
        }
        break;
    }
    case BanRuleId::B5:  //P believes Q believes (X,Y) -> P believes Q believes Y
    case BanRuleId::B7:  //P believes Q said (X,Y) -> P believes Q said Y
    case BanRuleId::J:   //P believes Q controls X , P believes Q believes X -> P believes X
    { goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
        for(int k=0; k<sList.count(); k++)
        {
            if(sList.value(k)->getStype()== BanSComponentType::bStatement)
            {
                st = dynamic_cast<BanStatementList *>(sList.value(k));
                YList= dynamic_cast<BanDataList*>(st->getStList().value(1));
                break;
            }
        }
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(i));
            s->findData(YList,instantiations, false,true);
        }
        for (int i=0; i<instantiations.count(); ++i)
        {
            copy = dynamic_cast<BanPostulates*>(instPostulate->getCopy());
            otherPart.append(instantiations.at(i));
            // instantiate Y component in prerequisite with found instantiations
            YSt=dynamic_cast<BanStatementList*>(copy->getPrerequisites().at(0));
            XList=dynamic_cast<BanDataList*>(dynamic_cast<BanStatementList*>(YSt->getStList().at(1))->getStList().at(1));
            XList->instantiateAnyData(otherPart, op,1);
            if (!ipl.contains(copy)) {
                ipl.append(copy);
                instPostulates=true;
            }
            cout<< copy->getPrerequisites().at(0)->getString().toStdString();
            while(!otherPart.isEmpty())
            {
               otherPart.pop();
            }
        }
        break;
    }
    case BanRuleId::B3://P believes (X,Y) -> P believes Y
    case BanRuleId::S6://P sees <X>Y -> P sees Y
    case BanRuleId::S7://P sees (X,Y) -> P sees Y
    { goalBan = instPostulate->getBanGoal();
        QList<BanSComponent*> sList=goalBan->getStList();
        for(int k=1; k<sList.count(); k++)
        {
            if(sList.value(k)->getStype()== BanSComponentType::bData)
            {
                de = dynamic_cast<BanDataList *>(sList.value(k));
                YList= dynamic_cast<BanDataList*>(de->getDataList().value(1));
                break;
            }
        }
        for (int i=0; i<lpt->getKnownTrueStatementCount(); ++i) {
            s = dynamic_cast<const BanStatementList *>(lpt->getKnownTrueStatement(i));
            s->findData(YList,instantiations, false,true);
        }
        for (int i=0; i<instantiations.count(); ++i)
        {
            copy = dynamic_cast<BanPostulates*>(instPostulate->getCopy());
            otherPart.append(instantiations.at(i));
            // instantiate Y component in prerequisite with found instantiations
            YSt=dynamic_cast<BanStatementList*>(copy->getPrerequisites().at(0));
            XList=dynamic_cast<BanDataList*>(YSt->getStList().at(1));
            XList->instantiateAnyData(otherPart, op,0);
            if (!ipl.contains(copy)) {
                ipl.append(copy);
                instPostulates=true;
            }
            cout<< copy->getPrerequisites().at(0)->getString().toStdString();
        }
        break;
    }
    case BanRuleId::S4://P believes P has K+ as a public key, P sees {X}K+ -> P sees X
    case BanRuleId::S3://P believes K is shared with Q, P sees {X}K -> P sees X
    case BanRuleId::S5://P believes Q has K+ as a public key, P sees {X}K- -> P sees X
    case BanRuleId::I1://P believes key K is shared with Q , P sees {X}K ->  P believes Q said X
    case BanRuleId::I2:  //P believes Q has public key K+ , P sees {X}K- ->  P believes Q said X
    case BanRuleId::I3:  //P believes Y is shared secret between Q and P , P sees <X>Y ->  P believes Q said X
    case BanRuleId::B8:  //P believes X, P believes Q believes (X,Y) -> P believes (X,Q)
    { if (!ipl.contains(instPostulate))
        {
            instPostulates=true;
            ipl.append(instPostulate);
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

    //7.1 : P believes X,  P believes Y -> P  (X  Y concates)believes
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
