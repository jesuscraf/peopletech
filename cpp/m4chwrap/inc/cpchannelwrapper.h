
//////////////////////////////////////////////////////////////////////////////
// CProxy_WrapAccessEvents
template <class T>
class CProxy_WrapAccessEvents : public IConnectionPointImpl<T, &DIID__WrapAccessEvents, CComDynamicUnkArray>
{
public:
//methods:
//_WrapAccessEvents : IDispatch
public:
	void Fire_Invalidate()
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	}
	void Fire_Refresh()
	{
		T* pT = (T*)this;
		pT->Lock();
		IUnknown** pp = m_vec.begin();
		while (pp < m_vec.end())
		{
			if (*pp != NULL)
			{
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(*pp);
				pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
			pp++;
		}
		pT->Unlock();
	}

};

