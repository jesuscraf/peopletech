
#ifndef __COMPST_FAC_HPP__
#define __COMPST_FAC_HPP__

class _AClComponentStore;
class ClNode_Data;

class ClComponentStore_Factory
{
public:
	static void CreateComponentStore (ClNode_Data *);
    static void Update (ClNode_Data *ai_pnd);
};

#endif //__COMPST_FAC_HPP__
