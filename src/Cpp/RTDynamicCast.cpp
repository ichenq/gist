
//only ptrdiff_t and size_t allowed
template <typename R>
R   ReadData(size_t ptr)
{
    return *((const R*)ptr);
}

template <typename R, typename T>
R   ReadData(const T* ptr)
{
    return ReadData<R>((size_t)ptr);
}



// We assume the vftable is at offset 0
// If it is not the case, we assume the vbtable should be at offset 0,
// and with its first entry equal to 0(point to itself)
// Then its second entry should contain the offset of vftable
template <typename T>
_s_RTTICompleteObjectLocator*   GetCompleteObjectLocator(const T* ptr) // not have vbtable
{
    ptrdiff_t offset = 0;
    if (ReadData<ptrdiff_t>(ReadData<size_t>(ptr)) == 0)
    {
        offset = ReadData<ptrdiff_t>(ReadData<size_t>(ptr) + 4);
    }
    return (_s_RTTICompleteObjectLocator*)(ReadData<size_t>(
        ReadData<size_t>((size_t)ptr + offset) - 4));
}


template <typename T>
void*   FindCompleteObject(const T* ptr)
{
    const _s_RTTICompleteObjectLocator* pCompleteObjectLocator = GetCompleteObjectLocator(ptr);
    if (pCompleteObjectLocator)
    {
        ptr = (T*)((size_t)ptr - pCompleteObjectLocator->offset);
        if (pCompleteObjectLocator->cdOffset)
        {
            ptr -= *(ptr - pCompleteObjectLocator->cdOffset);
        }
    }
    return ptr;
}


void*   __RTCastToVoid(const void* ptr)
{
    if (!ptr)
        return NULL;
    return FindCompleteObject(ptr);
}


template <typename T>
const TypeDescriptor*   __RTtypeid(const T* ptr)
{
    if (!ptr)
    {
        throw bad_typeid("Attempted a typeid of NULL pointer");
        const _s_RTTICompleteObjectLocator* pCompleteLocator = GetCompleteObjectLocator(ptr);
        TypeDescriptor* pTypeDescriptor = pCompleteLocator->pTypeDescriptor;
        if (!pTypeDescriptor)
        {
            throw std::__non_rtti_object("Bad read pointer - no RTTI data!");
        }
        return pTypeDescriptor;
    }
}

template <typename T>
const type_info&    GetTypeID(const T* obj)
{
    if (typeid(*obj) == *__RTtypeid(obj))
    {
        return typeid(*obj);
    }
    else
    {
        throw std::bad_typeid("...");
    }
}


template <typename U>
U*      __RTDynamicCast(const void* ptr,
                        ptrdiff_t offset,
                        const TypeDescriptor* fromType,
                        const TypeDescriptor* toType,
                        int isReference)
{
    void* pCompleteObject = FindCompleteObject(ptr);
    ptrdiff_t inptr_delta = (size_t)ptr - offset - (size_t)pCompleteObject;
    const _s_RTTICompleteObjectLocator* pCompleteLocator = GetCompleteObjectLocator(ptr);
    int flag = pCompleteLocator->pClassHierarchyDescriptor->attributes;
    const _s_RTTIBaseClassDescriptor* pBaseClassDescriptor = NULL;
    if (fromType == toType)
    {
        pBaseClassDescriptor = pCompleteLocator->pClassHierarchyDescriptor->pBaseClassArray[0];
    }
    else
    {
        if ((flag & 1) == 0)
        {
            pBaseClassDescriptor = FindSITargetTypeInstance(pCompleteLocator, fromType, toType);
        }
        else if ((flag & 2) == 0)
        {
            pBaseClassDescriptor = FindMITargetTypeInstance(pCompleteObject, pCompleteLocator,
                fromType, inptr_delta, toType);
        }
        else
        {
            pBaseClassDescriptor = FindVITargetTypeInstance(pCompleteObject, pCompleteLocator,
				fromType, inptr_delta, toType);
        }
    }
    if (!pBaseClassDescriptor && isReference)
    {
        throw std::bad_cast("Bad dynamic_cast!");
    }
    else
    {
        size_t addr = 0;
        if (pBaseClassDescriptor)
        {
            addr = (size_t)pCompleteObject + PMDtoOffset(pCompleteObject, pBaseClassDescriptor->pmd);
        }
        return (void*)addr;
    }
}
