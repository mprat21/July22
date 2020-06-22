#include "bandatac.h"
#include "bandatom.h"
using namespace BANLogic;
int BANLogic::BanDAtom::atomCount=1;

banAtomtype BANLogic::BanDAtom::getAtype() const
{
    return atype;
}

void BANLogic::BanDAtom::setAtomtype(banAtomtype aTy)
{
    atype=aTy;
}

void BANLogic::BanDAtom::setDtype(QString aVal)
{
    if(aVal=="A" ||aVal=="B"||aVal=="C"||aVal.contains("P")||aVal=="S")
    {
        atype=Principal;
    }
    else if(aVal=="Ka" ||aVal=="Kc"||aVal=="Ks"||aVal=="Kb")
    {
        atype=PubKey;
    }
    else if(aVal=="Na" ||aVal=="Nb"||aVal=="Nc"||aVal=="Np" || aVal=="Ns" )
    {
        atype=Nonce;
    }
    else if(aVal=="Ta" ||aVal=="Tb"||aVal=="Tc"||aVal=="Tp" || aVal=="Ts" )
    {
        atype=Timestamp;
    }
    else if(aVal=="Kab" ||aVal=="Kac"||aVal=="Kas"||aVal=="Kbc" || aVal=="Kbs" || aVal=="Kcs" )
    {
        atype=PubKey;
    }
    aValue=aVal;
}

bool BANLogic::BanDAtom::match(BanDComponent *value)
{
    ifMatches=true;
    if(value!=NULL){
        switch(value->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom *bad = dynamic_cast<BanDAtom *>(value);
            if(bad->getAtype()!=this->getAtype())
            {
                ifMatches=false;
            }
            else if(this->instantiate && bad->instantiate)
            {
                ifMatches=this->aValue==bad->aValue;
            }
        }
            break;
        case BanDComponentType::bOperator:
        {
            ifMatches=false;
        }
            break;
        case BanDComponentType::bAnyData:
        {
            BanDataC *d =dynamic_cast<BanDataC *>(value);
            QList<BanDComponent*> dlist=d->getMyListdata();
            for(int i=0; i<dlist.size();i++)
            {
                switch(dlist.value(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    ifMatches = match(dlist.value(i));
                    break;
                }
                case BanDComponentType::bOperator:
                {
                    ifMatches = false;
                    break;
                }
                }
            }
            break;
        }
        default:
            throw new BanException("Unrecognised Component Type in banAtom::match(...)");
        }
    }
    else ifMatches=false;
    return ifMatches;
}

bool BANLogic::BanDAtom::unify(BanDComponent *value)
{
    if(this->match(value))
    {
        switch(value->getDtype())
        {
        case BanDComponentType::bAtom:
        {
            if(this->getID()!=value->getID())
            {
                if(this->getInstantiate()==false && value->getInstantiate()==true)
                {
                    cout<<endl;

                    QTextStream(stdout)<<this->getID()<<" = " <<value->getID() <<endl;
                    this->setId(value->getID());
                    this->unifies=true;
                }
                else if(this->getInstantiate()==true && value->getInstantiate()==false)
                {
                    cout<<endl;

                    QTextStream(stdout) << value->getID()<<" = "<<this->getID() <<endl;
                    value->setId(this->getID());
                    this->unifies=true;
                }
                else if(this->getInstantiate()==false && value->getInstantiate()==false)
                {
                    cout<<endl;

                    QTextStream(stdout) << this->getID()<<" = "<<value->getID() <<endl;
                    this->setId(value->getID());
                    this->unifies=true;
                }
            }
            else if(this->getID()==value->getID())
            {
                this->setId(value->getID());
                this->unifies=true;
            }
            break;
        }
        case BanDComponentType::bOperator:
        {
            this->unifies=false;
            break;
        }
        case BanDComponentType::bAnyData:
        {
            if(value->getInstantiate()==false)
            {
                this->unifies=true;
                value->setId(this->getID());
                cout<<endl;
                QTextStream(stdout)<< value->getID()<<" = "<<this->getID() <<endl;
            }
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDAtom::unify()");
        }
        }
    }
    return this->unifies;
}

void BANLogic::BanDAtom::setId(const QString &value)
{
    this->aValue = value;
}

bool BANLogic::BanDAtom::getInstantiate() const
{
    return instantiate;
}

bool BanDAtom::getIfMatches() const
{
    return ifMatches;
}

BanDAtom::~BanDAtom()
{
}


BANLogic::BanDAtom::BanDAtom():BanDComponent(BanDComponentType::bAtom)
{
}

BANLogic::BanDAtom::BanDAtom(banAtomtype aTy, QString aVal):BanDComponent(BanDComponentType::bAtom),atype(aTy)
{
    if(aVal==NULL || aVal.isEmpty())
    {
        switch (aTy)
        {
        case banAtomtype::Principal :
        {
            this->aValue=QString("_P_%1").arg(atomCount++);
            this->atype=banAtomtype::Principal;
            break;
        }
        case banAtomtype::Nonce:
        {
            this->aValue=QString("_N_%1").arg(atomCount++);
            this->atype=banAtomtype::Nonce;
            break;
        }
        case banAtomtype::PubKey:
        {
            this->aValue=QString("_K+_%1").arg(atomCount++);
            this->atype=banAtomtype::PubKey;
            break;
        }
        case banAtomtype::SymKey:
        {
            this->aValue=QString("_K_%1").arg(atomCount++);
            this->atype=banAtomtype::SymKey;
            break;
        }
        case banAtomtype::PrivKey:
        {
            this->aValue=QString("_K-_%1").arg(atomCount++);
            this->atype=banAtomtype::PrivKey;
            break;
        }
        case banAtomtype::Function:
        {
            this->aValue=QString("_F_%1").arg(atomCount++);
            this->atype=banAtomtype::Function;
            break;
        }
        case banAtomtype::Hash:
        {
            this->aValue=QString("_H_%1").arg(atomCount++);
            this->atype=banAtomtype::Hash;
            break;
        }
        case banAtomtype::Timestamp:
        {
            this->aValue=QString("_TS_%1").arg(atomCount++);
            this->atype=banAtomtype::Timestamp;
            break;
        }
        case banAtomtype::BinaryData:
        {
            this->aValue=QString("_"+aVal+"_%1").arg(atomCount++);
            this->atype=banAtomtype::BinaryData;
            break;
        }
        default:
        {
            throw new BanException("Unrecognised Component Type in banDAtom::constructor()");
        }
        }
        instantiate=false;
    }
    else
    {
        this->aValue = aVal;
        this->atype= aTy;
        instantiate = true;
        atomCount++;
    }
}


