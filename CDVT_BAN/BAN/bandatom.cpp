#include "bandatac.h"
#include "bandatom.h"
using namespace BANLogic;
namespace BANLogic {

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

bool BANLogic::BanDAtom::match(BanDComponent& value)
{
    ifMatches=true;
    if(value.getInstantiate()==true){
        switch(value.getDtype())
        {
        case BanDComponentType::bAtom:
        {
            BanDAtom &bad = dynamic_cast<BanDAtom&>(value);
            if(bad.getAtype()!=this->getAtype())
            {
                ifMatches=false;
            }
            else if(this->instantiate && bad.instantiate)
            {
                ifMatches=this->aValue==bad.aValue;
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
            BanDataC &d =dynamic_cast<BanDataC&>(value);
            QList<BanDComponent*> dlist=d.getMyListdata();
            for(int i=0; i<dlist.size();i++)
            {
                switch(dlist.value(i)->getDtype())
                {
                case BanDComponentType::bAtom:
                {
                    ifMatches = match(*dlist.value(i));
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
void BanDAtom::instantiateObject(BanDComponent *value)
{
    BanDAtom *a =dynamic_cast<BanDAtom*>(value);
    this->setId(a->getID());
    this->inverseKey = a->inverseKey;
    this->instantiate=a->instantiate;
}

bool BANLogic::BanDAtom::unify(BanDComponent& value)
{
    if(this->match(value))
    {
        switch(value.getDtype())
        {
        case BanDComponentType::bAtom:
        {
            if(this->getID()!=value.getID())
            {
                if(this->getInstantiate()==false && value.getInstantiate()==true)
                {
                    BanDAtom &a =dynamic_cast<BanDAtom&>(value);
                    std::cout<<this->getID().toStdString()<<" = " <<a.getID().toStdString() <<endl;
                   this->instantiateObject(&value);
                    this->unifies=true;
                }
                else if(this->getInstantiate()==true && value.getInstantiate()==false)
                {
                    cout<<endl;
                    BanDAtom &a =dynamic_cast<BanDAtom&>(value);

                    QTextStream(stdout) << value.getID()<<" = "<<this->getID() <<endl;
                    a.instantiateObject(this);
                    a.unifies=true;
                }
                else if(this->getInstantiate()==false && value.getInstantiate()==false)
                {
                    cout<<endl;

                    QTextStream(stdout) << this->getID()<<" = "<<value.getID() <<endl;
                    this->instantiateObject(&value);
                    this->unifies=true;
                }
            }
            else if(this->getID()==value.getID())
            {
               this->instantiateObject(&value);
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
            if(value.getInstantiate()==false)
            {
                this->unifies=true;
                value.setId(this->getID());
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


BanDAtom *BanDAtom::getInverseKey() const
{
    return inverseKey;
}

void BanDAtom::setInverseKey(BanDAtom *value)
{
    if ( (atype==banAtomtype::PubKey) || (atype==banAtomtype::PrivKey))
        inverseKey = value;
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
            this->aValue=QString("_P%1_").arg(atomCount++);
            this->atype=banAtomtype::Principal;
            break;
        }
        case banAtomtype::Nonce:
        {
            this->aValue=QString("_N%1_").arg(atomCount++);
            this->atype=banAtomtype::Nonce;
            break;
        }
        case banAtomtype::PubKey:
        {
            this->aValue=QString("_K%1+_").arg(atomCount++);
            this->atype=banAtomtype::PubKey;
            break;
        }
        case banAtomtype::SymKey:
        {
            this->aValue=QString("_K%1_").arg(atomCount++);
            this->atype=banAtomtype::SymKey;
            this->inverseKey = this;
            break;
        }
        case banAtomtype::PrivKey:
        {
            this->aValue=QString("_K%1-_").arg(atomCount++);
            this->atype=banAtomtype::PrivKey;
            break;
        }
        case banAtomtype::Function:
        {
            this->aValue=QString("_F%1_").arg(atomCount++);
            this->atype=banAtomtype::Function;
            break;
        }
        case banAtomtype::Hash:
        {
            this->aValue=QString("_H%1_").arg(atomCount++);
            this->atype=banAtomtype::Hash;
            break;
        }
        case banAtomtype::Timestamp:
        {
            this->aValue=QString("_TS%1_").arg(atomCount++);
            this->atype=banAtomtype::Timestamp;
            break;
        }
        case banAtomtype::BinaryData:
        {
            this->aValue=QString("_Y%1_").arg(atomCount++);
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

BANLogic::BanDAtom::BanDAtom(BanDAtom &orig):BanDComponent(BanDComponentType::bAtom)
{
    setId(orig.getID());
    atype = orig.atype;
    setInstantiate(orig.getInstantiate());
    inverseKey = orig.inverseKey;
    //allAtom.append(this);
}

}


QString BANLogic::BanDAtom::getString()
{
    return this->aValue;
}

bool BANLogic::BanDAtom::operator ==(const BanDComponent &other)
{
    bool equals = false;
    if(other.getDtype()==BanDComponentType::bAtom)
    {
        const BanDAtom &a = dynamic_cast<const BanDAtom &>(other);
        if (atype != a.atype) {
            equals = false;
        } else {
            if(aValue == a.aValue) equals = true;
        }
    }
    else if(other.getDtype()==BanDComponentType::bAnyData && instantiate==false) equals=true;
    return equals;
}

