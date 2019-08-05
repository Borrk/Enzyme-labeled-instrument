#ifndef __ENM_MATHPARSER_H_
#define __ENM_MATHPARSER_H_

#include "EnmBase.hpp"

enum enOperatorPriority{ PRI_NONE = 0, PRI_R_BRACKET, PRI_ADD, PRI_MUL, PRI_L_BRACKET };
enum enTockenType{ TK_NONE, TK_OPERATOR, TK_VARIABLE, TK_NUMBER, TK_ERROR };

class CTocken
{
/// member routines
public:
	CTocken()
	{
		mOperation.oprnd = 0;
		mPriority = PRI_NONE;
		mType = TK_NONE;
	}

	CTocken( FP64 operand )
	{
		mOperation.oprnd = operand;
		mType = TK_NUMBER;
	}

	CTocken( MCHAR var )
	{
		mOperation.var = var;
		mType = TK_VARIABLE;
	}

	CTocken( MCHAR op, INT8U pri )
	{
		mOperation.optor = op;
		mType = TK_OPERATOR;
		mPriority = pri;
	}

	static enOperatorPriority GetPriority( MCHAR ch )
	{
		if ( ch == '+' || ch == '-' )
		{
			return PRI_ADD;
		}
		else if ( ch == '*' || ch == '/' )
		{
			return PRI_MUL;
		}
		else if ( ch == '(' )
		{
			return PRI_L_BRACKET;
		}
		else if ( ch == ')' )
		{
			return PRI_R_BRACKET;
		}
		else
		{
			return PRI_NONE;
		}
	}

	/// member variables
	INT8U	mPriority;	
	INT8U	mType;
	union
	{
		FP64	oprnd;
		MCHAR	optor;
		MCHAR	var;
	}	mOperation;
};


/**
 *
 *  class CStack
 *
 */
class CStack
{
/// member routines
public:
	CStack();
	~CStack();

	CTocken& Pop();
	void	push( const CTocken& tocken );
	void	Clear();
	CTocken& GetAt( INT16U idx );

	INT16U	GetSize();

private:
	void Init();

/// member variables
private:
	/// now temp fix size stack, in ARM7 platform
	enum { STACK_SIZE = 128 };
	CTocken		mTockenStack[STACK_SIZE];
	INT16U		mPointer;
	INT16U		mStackSize;
};


/**
 *
 *  class CMathCalculater
 *
 */
class CMathCalculater
{
/// member routines
public:
	CMathCalculater();
	~CMathCalculater();

	BOOLEAN	Calculate( MCHAR* pszExpression, FP64& result );	///< TRUE: calculate ok, FALSE: failed
	BOOLEAN ValidateExpression( MCHAR* pszExpression, BOOLEAN bHaveVariable = TRUE );

private:
	void Init();
	INT16U	GetTocken( MCHAR* pszExpression );
	INT16S	GetOneTocken( MCHAR* pszExpression, CTocken& tocken );
	BOOLEAN  CalExp( FP64& val );

/// member variables
private:

	CStack	mStack;
};

#endif ///< __ENM_MATHPARSER_H_