#ifndef  __EnmPgEtSampleList_H
#define  __EnmPgEtSampleList_H


#include "MIG_EditBox.Hpp"

#define  STD_SHOW_POS  1

class CEnmSampleList : public MigGrid
{
/// member routines
public:
	CEnmSampleList( MigObj *pParent = NULL );
	CEnmSampleList( const tagCS& cs,  const tagGridBoxCS& bcs );

	INT16U	AddSample( tagSampleData& data );
	INT16U	AddSample( INT16U sample );
	void	DeleteSample( INT16U sample );
	void	DeleteAllSample();
	void	SetOneSample( INT16U ln, tagSampleData& data );
	void	SetOneSample( INT16U ln, INT16U sample );
	void	ClearOneSample( INT16U ln );
	void	SetCurSample( INT16U sample ) { ItemFocusOn( sample, 1 ); }
	BOOLEAN	GetOneSample( INT16U ln, tagSampleData& data );

protected:	
	virtual void	OnKey( INT16U key );
	virtual void	OnKeyOK(void);

	virtual void	Layout( void);	///< set layout by current line and comumn
	void	SetList(void);

public:
	void	SetRefProtocol( tagProtocol* pProtocol ) { mpRefProtocol = pProtocol; }

/// member variables
protected:
	tagProtocol*	mpRefProtocol;
};


#endif ///< __EnmPgEtSampleList_H
