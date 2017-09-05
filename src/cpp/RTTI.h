#include <typeinfo>


typedef std::type_info  TypeDescriptor;


struct PMD
{
    ptrdiff_t   mdisp;      // vftable offset
    ptrdiff_t   pdisp;      // vbtable offset
    ptrdiff_t   vdisp;      // vftable offset(for virtual base class)
    
    void    Output()
    {
        printf("(%u, %u, %u)", mdisp, pdisp, vdisp);
    }
};



struct _s_RTTICompleteObjectLocator;
struct _s_RTTIClassHierarchyDescriptor;
struct _s_RTTIBaseClassDescriptor;


struct _s_RTTICompleteObjectLocator
{
    DWORD   signature;
    DWORD   offset; // vftable offset to this
    DWORD   cdOffset;
    TypeDescriptor* pTypeDescriptor;
    _s_RTTIClassHierarchyDescriptor*    pClassHierarchyDescriptor;

    void    Output(size_t tabs = 0);
};


struct _s_RTTIClassHierarchyDescriptor
{
    DWORD   signature;
    DWORD   attributes; // bit 0 multiple inheritance, bit 1 virtual inheritance
    size_t  numBaseClasses; // at least 1(all base classes, including itself)
    _s_RTTIBaseClassDescriptor** pBaseClassArray;

    void    Output(size_t tabs = 0);
};


struct _s_RTTIBaseClassDescriptor
{
    TypeDescriptor* pTypeDescriptor;
    size_t  numBaseClasses; // direct base classes
    PMD     pmd;    // len = 0xC
    DWORD   attributes;
    _s_RTTIClassHierarchyDescriptor* pClassHierarchyDescriptor;

    void    Output(size_t tabs = 0);
};


inline void OutputTab(size_t tabs = 0)
{
    for (size_t i = 0; i < tabs; ++i)
    {
        putchar('\t');
    }
}


inline void _s_RTTICompleteObjectLocator::Output(size_t tabs)
{
    OutputTab(tabs);
    printf("CompleteObjectLocator: \n");
    OutputTab(tabs);
    printf("signature: %u\n", signature);
    OutputTab(tabs);
    printf("offset: %u\n", offset);
    OutputTab(tabs);
    printf("cdOffset: %u\n", cdOffset);
    OutputTab(tabs);
    printf("pTypeDescriptor: %s\n", pTypeDescriptor->name());
    OutputTab(tabs);
    printf("pClassHierarchyDescriptor: \n");
    pClassHierarchyDescriptor->Output(tabs+1);
    printf("\n\n");
}


inline void _s_RTTIClassHierarchyDescriptor::Output(size_t tabs)
{
    OutputTab(tabs);
    printf("signature: %u\n", signature);
    OutputTab(tabs);
    printf("attributes: %u\n", attributes);
    for (size_t i = 0; i < numBaseClasses; ++i)
    {
        OutputTab(tabs);
        printf("base class%u: \n", i+1);
        pBaseClassArray[i]->Output(tabs+1);
    }
}


inline void _s_RTTIBaseClassDescriptor::Output(size_t tabs)
{
    OutputTab(tabs);
    printf("pTypeDescriptor: %s\n", pTypeDescriptor->name());
    OutputTab(tabs);
    printf("numBaseClasses: %u\n", numBaseClasses);
    OutputTab(tabs);
    printf("pmd: ");
    pmd.Output();
    printf("\n");
    OutputTab(tabs);
    printf("attributes: %u\n", attributes);
}