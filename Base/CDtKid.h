#ifndef CDTKID_H_HEADER_INCLUDED_BBE450A1
#define CDTKID_H_HEADER_INCLUDED_BBE450A1

#include "EnmBase.hpp"

class IWorkStep;

#define  KID_MAX  10

//##ModelId=441BBC4F036B
class CDtKid
{
  public:

    CDtKid();
	~CDtKid();
	
	U16 GetKidCount() const;

    //##ModelId=441BBC9F0119
    IWorkStep* GetKidAt(U16 idx) const;

    //##ModelId=441BBCA5009C
    S16 AddKidAt(IWorkStep& pStep);
	S16 SetKidAt(IWorkStep& pStep, U16 idx );

    //##ModelId=441BBD2C005D
    BOOLEAN RemoveKid(U16 idx);

  protected:
    //##ModelId=441BBC79001F
    IWorkStep* mKidList[KID_MAX];

    //##ModelId=441BBC7F0242
    U16 mKidCount;
	U16 mKidMax;
};

//##ModelId=441BBC95031C
inline U16 CDtKid::GetKidCount() const
{
	return mKidCount;
}



#endif /* CDTKID_H_HEADER_INCLUDED_BBE450A1 */
