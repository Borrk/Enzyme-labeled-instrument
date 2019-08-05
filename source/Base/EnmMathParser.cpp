#include <string.h>
#include "EnmMathParser.hpp"

CStack::CStack()
{
	mStackSize = STACK_SIZE;
	Init();
}
CStack::~CStack()
{
	
}

CTocken& CStack::Pop()
{
	if ( mPointer == 0 )
	{
		mTockenStack[0].mType = TK_NONE;
		return mTockenStack[0];
	}
	else
	{
		mPointer--;
		return mTockenStack[mPointer];
	}

}
	
void CStack::push( const CTocken& tocken )
{
	if ( mPointer < mStackSize - 1 )
	{
		mTockenStack[mPointer++] = tocken;
	}
}
	
void CStack::Clear()
{
	mPointer = 0;
}	

CTocken& CStack::GetAt( INT16U idx )
{
	if ( idx >= mPointer )
	{
		mTockenStack[idx].mType = TK_NONE;
		return mTockenStack[idx];
	}
	else
	{
		return mTockenStack[idx];
	}
}

INT16U CStack::GetSize()
{
	return mPointer; //mStackSize;
}

void CStack::Init()
{
	mPointer = 0;
}


/**
 *
 *  CMathCalculater
 *
 */

CMathCalculater::CMathCalculater()
{
}

CMathCalculater::~CMathCalculater()
{
}

BOOLEAN  CMathCalculater::CalExp( FP64& val )
{
	val = 0;

	MCHAR  opr;
	FP64 aPreVal, aPostVal;
	CTocken  aTocken;
	BOOLEAN aRet = TRUE, aHasBracked = FALSE;

	aTocken = mStack.Pop();

	if ( aTocken.mType == TK_OPERATOR )
	{
		if ( aTocken.mOperation.optor == ')' )
		{
			aTocken = mStack.Pop();
			aHasBracked = TRUE;
		}
		else
			return FALSE;
	}


	/// Get next operand
	if ( aTocken.mType == TK_NUMBER )
	{
		aPostVal = aTocken.mOperation.oprnd;
	}
	else
		return FALSE;

	/// get operator
	aTocken = mStack.Pop();
	if ( aTocken.mType == TK_OPERATOR )
	{
		opr = aTocken.mOperation.optor;
	}
	else
		return FALSE;

	/// get previous operand
	aTocken = mStack.Pop();
	if ( aTocken.mType == TK_NUMBER )
	{
		aPreVal = aTocken.mOperation.oprnd;
	}
	else
		return FALSE;

	/// calculate
	switch( opr )
	{
	case '+':
		val = aPreVal + aPostVal;
		break;
	case '-':
		val = aPreVal - aPostVal;
		break;
	case '*':
		val = aPreVal * aPostVal;
		break;
	case '/':
		if ( aPostVal == 0.0 )
		{
			val = 0.0;
			aRet = FALSE;
		}
		else
			val = aPreVal / aPostVal;
		break;

	default:
		aRet = FALSE;
	}

	/// pop the left bracket
	if ( aHasBracked )
		mStack.Pop();

    return aRet;
}

BOOLEAN	CMathCalculater::Calculate( MCHAR* pszExpression, FP64& result )
{
	BOOLEAN bRet = FALSE;

	INT16U i;
	INT16U aCnt = GetTocken( pszExpression );

	INT16U  aErr, aLeftBracket;
	BOOLEAN bBracket;
	CStack  aStack;
	CTocken  aTocken, aPreTocken, aPreOp;

	i = 0;
	while ( i++ < aCnt )
	{
		aStack.push( mStack.Pop() );
	}
/// TK_NONE, TK_OPERATOR, TK_VARIABLE, TK_NUMBER, TK_ERROR

	FP64 aVal, aPreVal, aPostVal;
	i = 0;
	aErr = 0;
	bBracket = FALSE;
	mStack.Clear();
	aPreOp.mType = TK_NONE;
	while ( i++ < aCnt )
	{
		aTocken = aStack.Pop();

		/// check whether expression is valid
		// add later

		switch ( aTocken.mType )
		{
		case TK_OPERATOR:
			if ( aPreOp.mType == TK_NONE )
			{
				mStack.push( aTocken );
				aPreOp = aTocken;
			}
			else
			{
				if ( aTocken.mPriority <= aPreOp.mPriority && aPreOp.mOperation.optor != '(' )
				{
					if ( aTocken.mOperation.optor == ')' )
						mStack.push( aTocken );

					if(  CalExp(aVal) )
					{
						/// push back 
						if ( aTocken.mOperation.optor != ')' )
							aStack.push( aTocken );

						aTocken.mType = TK_NUMBER;
						aTocken.mOperation.oprnd = aVal;
						aStack.push( aTocken );
						
						aTocken = mStack.Pop();
						while ( aTocken.mType != TK_NONE )
						{
							aStack.push( aTocken );
							aTocken = mStack.Pop();
						}

						i = 0;
						aErr = 0;
						bBracket = FALSE;
						aPreOp.mType = TK_NONE;
						mStack.Clear();
						aCnt = aStack.GetSize();
					}
					else
					{
						aErr = 1;
					}
				}
				else
				{
					mStack.push( aTocken );
					aPreOp = aTocken;
				}
			}
			break;

		case TK_NUMBER:
			mStack.push( aTocken );
			
			/// check if is the last number
			if ( aStack.GetSize() == 0 )
			{
				if(  CalExp(aVal) )
				{
					result = aVal;
					i = aCnt;
					bRet = TRUE;
				}
				else
					aErr = 1;
			}
			break;

		default:
			aErr = 1;
			break;
		}

		aPreTocken = aTocken;

		if ( aErr )
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

BOOLEAN CMathCalculater::ValidateExpression( MCHAR* pszExpression, BOOLEAN bHaveVariable  )
{
	BOOLEAN bRet = FALSE;

	return bRet;
}

void Init()
{
}

INT16U	CMathCalculater::GetTocken( MCHAR* pszExpression )
{
	INT16S aPos = 0;

	mStack.Clear();

	if ( pszExpression == NULL )
		return 0;

	CTocken aTocken;
	//strstr( pszExpression );
	INT16U aLen = strlen( pszExpression );
	MCHAR  *ap = pszExpression;

	while ( ap - pszExpression  < aLen )
	{
		aPos = GetOneTocken( ap, aTocken );
		if ( aPos == 0 )
			break;

		ap += aPos;
		mStack.push( aTocken );
	}

	return mStack.GetSize();
}

INT16S	CMathCalculater::GetOneTocken( MCHAR* pszExpression, CTocken& tocken )
{
	INT16S aPos = 0;
	INT16S aLen = strlen( pszExpression );

	enTockenType  aTockenType = TK_NONE; ///< TK_NONE: none,  TK_OPERAND: operand, 
						///< TK_VARIABLE: variables, TK_NUMBER: number

	INT16S aIdx = 0;
	MCHAR ch, temp, aBuf[32] = { 0 };
	while ( aPos < aLen )
	{
		ch = pszExpression[aPos];
		if ( ch == ' ' )
		{
			aPos++;
			continue;
		}

		switch( aTockenType )
		{
		case TK_NONE:
			if ( strchr("+-*/()", ch) )
			{
				aTockenType = TK_OPERATOR;
				tocken.mOperation.optor = ch;
				tocken.mPriority = CTocken::GetPriority( ch );
				tocken.mType = TK_OPERATOR;
			}
			else if ( strchr("PNL", ch) )
			{
				if ( aPos + 1 == aLen )	///< the last one
				{
					aTockenType = TK_VARIABLE;
				}
				else
				{
					temp = pszExpression[aPos+1];
					if ( strchr("+-*/)", temp) ) ///< followed by operand
					{
						aTockenType = TK_VARIABLE;
					}
					else
					{
						aTockenType = TK_ERROR;
					}
				}

				if ( aTockenType == TK_VARIABLE )
				{
					tocken.mOperation.var = ch;
					tocken.mType = TK_VARIABLE;
				}
			}
			else if ( strchr("1234567890", ch) )
			{
				aTockenType = TK_NUMBER;
				aIdx = 0;
				aBuf[aIdx++] = ch;
			}
			else
			{
				aTockenType = TK_ERROR;
			}
			break;

		case TK_NUMBER:
			if ( strchr("+-*/)", ch) )
			{
				aBuf[aIdx] = '\0';
				aTockenType = TK_NONE;
				aPos--;

				tocken.mOperation.oprnd = Str2FP( aBuf ); //atof( aBuf ); 
				tocken.mType = TK_NUMBER;
			}
			else if ( strchr("1234567890.", ch) )
			{
				aBuf[aIdx++] = ch;
			}
			else
			{
				aTockenType = TK_ERROR;
			}

			break;
	
		case TK_OPERATOR:
		case TK_VARIABLE:
        default:
			aTockenType = TK_ERROR;
		}

		aPos++;
		if ( aTockenType == TK_ERROR )
			aPos = 0;

		if ( aTockenType != TK_NUMBER )
			return aPos;
	}

	if ( aPos >= aLen && aTockenType == TK_NUMBER )
	{
		aBuf[aIdx] = '\0';
		tocken.mOperation.oprnd = Str2FP( aBuf ); //atof( aBuf );
		tocken.mType = TK_NUMBER;
	}

	return aPos;
}

