
struct ColourValues
{
    int value;
    char* code;
    struct ColourValues *next;
};
typedef struct ColourValues *CV;
static CV CHead,CCurrent,CCurrent2,CNewNode;
static CV FHead,FCurrent,FCurrent2,FNewNode;


struct ValuesAndFrequencies
{
    int value;
    int frequency;
    struct ValuesAndFrequencies *next;
};
typedef struct ValuesAndFrequencies *VF;
static VF VFHead,VFCurrent,VFCurrent2,VFNewNode;


struct nodeTree
{
    int value;
    int frequency;
    int index;
    struct nodeTree *left;
    struct nodeTree *right;
};
typedef struct nodeTree *TRE;
static TRE TreeNodeNew;


struct liveNodeList
{
    TRE value;
    struct liveNodeList *next;
};
typedef struct liveNodeList *LN;
static LN LiveNodeHead,LiveNodeCurrent,LiveNodeCurrent2,LiveNodeNew,LiveNodePrevious;

