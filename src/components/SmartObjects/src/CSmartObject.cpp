#include "SmartObjects/CSmartObject.hpp"
#include <limits>
#include <errno.h>
#include <algorithm>



NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(void)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
}

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const NsAppLink::NsSmartObjects::CSmartObject& object)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    duplicate(object);
}

NsAppLink::NsSmartObjects::CSmartObject::~CSmartObject()
{
    cleanup_data();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const NsAppLink::NsSmartObjects::CSmartObject& uc)
{
    if (this != &uc) duplicate(uc);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(const NsAppLink::NsSmartObjects::CSmartObject& object) const
{
    if (m_type != object.m_type) return false;

    switch(m_type) {
        case SmartType_Integer :
            return m_data.long_value == object.m_data.long_value;
        case SmartType_Double :
            return m_data.double_value == object.m_data.double_value;
        case SmartType_Boolean :
            return m_data.bool_value == object.m_data.bool_value;
        case SmartType_Character :
            return m_data.char_value == object.m_data.char_value;
        case SmartType_String :
            return *(m_data.str_value) == *(object.m_data.str_value);
        case SmartType_Map :
            if (m_data.map_value == object.m_data.map_value) return true;
            return std::equal(m_data.map_value->begin(), m_data.map_value->end(), object.m_data.map_value->begin());
        case SmartType_Array :
            if (m_data.array_value == object.m_data.array_value) return true;
            return std::equal(m_data.array_value->begin(), m_data.array_value->end(), object.m_data.array_value->begin());
        case SmartType_Null :
            return true;
    }
    return false;
}

// =============================================================
// INTEGER TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(int i)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_integer(i);
}

NsAppLink::NsSmartObjects::CSmartObject::operator int(void) const
{
    return convert_int();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(int i)
{
    set_value_integer(i);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(int val) const
{
    int comp = convert_int();
    if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_integer(int l)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Integer);
    m_data.long_value = l;
}

int NsAppLink::NsSmartObjects::CSmartObject::convert_int(void) const
{
    long retval;

    switch (m_type) {
        case SmartType_String :
            retval = convert_string_to_long(m_data.str_value);
            break;
        case SmartType_Boolean:
            return (m_data.bool_value == true) ? 1 : 0;
        case SmartType_Integer:
            retval = m_data.long_value;
            break;
        case SmartType_Double :
            retval = static_cast<long>(m_data.double_value);
            break;
        default :
            return invalid_int_value;
    }

    if (retval > std::numeric_limits<int>::max() ||
        retval < std::numeric_limits<int>::min())
    {
        return invalid_int_value;
    }
    else
    {
        return static_cast<int>(retval);
    }
}

// =============================================================
// LONG TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(long l)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_long(l);
}

NsAppLink::NsSmartObjects::CSmartObject::operator long(void) const
{
    return convert_long();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(long l)
{
    set_value_integer(l);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(long val) const
{
    int comp = convert_long();
    if(comp == NsAppLink::NsSmartObjects::invalid_int_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_long(long l)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Integer);
    m_data.long_value = l;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_long(void) const
{
    switch (m_type) {
        case SmartType_String :
            return convert_string_to_long(m_data.str_value);
            break;
        case SmartType_Boolean:
            return (m_data.bool_value == true) ? 1 : 0;
        case SmartType_Integer:
            return m_data.long_value;
            break;
        case SmartType_Double:
            return static_cast<long>(m_data.double_value);
            break;
        default :
            break;
    }

    return invalid_int_value;
}

// =============================================================
// DOUBLE TYPE SUPPORT
// =============================================================
NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(double d)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_double(d);
}

NsAppLink::NsSmartObjects::CSmartObject::operator double(void) const
{
    return convert_double();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(double d)
{
    set_value_double(d);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(double val) const
{
    double comp = convert_double();
    if(comp == NsAppLink::NsSmartObjects::invalid_double_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_double(double d)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Double);
    m_data.double_value = d;
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_double(void) const
{
    switch (m_type) {
        case SmartType_String:
            return convert_string_to_double(m_data.str_value);
        case SmartType_Boolean:
            return (m_data.bool_value)? 1.0 : 0.0;
        case SmartType_Integer:
            return static_cast<double>(m_data.long_value);
        case SmartType_Double:
            return m_data.double_value;
        default:
            return invalid_double_value;
    }

    return invalid_double_value;
}

// =============================================================
// BOOL TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(bool b)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_bool(b);
}

NsAppLink::NsSmartObjects::CSmartObject::operator bool(void) const
{
    return convert_bool();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(bool b)
{
    set_value_bool(b);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(bool val) const
{
    bool comp = convert_bool();
    if(comp == NsAppLink::NsSmartObjects::invalid_bool_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_bool(bool b)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Boolean);
    m_data.bool_value = b;
}

bool NsAppLink::NsSmartObjects::CSmartObject::convert_bool(void) const
{
    switch (m_type) {
        case SmartType_Boolean :
            return m_data.bool_value;
        case SmartType_Integer :
            return (m_data.long_value != 0);
            break;
        case SmartType_Double :
            return (m_data.double_value != 0.0);
            break;
        default :
            break;
    }

    return invalid_bool_value;
}



// =============================================================
// CHAR TYPE SUPPORT
// =============================================================


NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char c)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_char(c);
}

NsAppLink::NsSmartObjects::CSmartObject::operator char(void) const
{
    return convert_char();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(char c)
{
    set_value_char(c);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(char val) const
{
    char comp = convert_char();
    if(comp == NsAppLink::NsSmartObjects::invalid_char_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_char(char c)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_Character);
    m_data.char_value = c;
}


char NsAppLink::NsSmartObjects::CSmartObject::convert_char(void) const
{
    switch (m_type) {
        case SmartType_String:
            return (m_data.str_value->length() == 1) ? m_data.str_value->at(0) : invalid_char_value;
        case SmartType_Character:
            return m_data.char_value;
        default :
            break;
    }

    return invalid_char_value;
}


// =============================================================
// STD::STRING TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(const std::string s)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_string(s);
}

NsAppLink::NsSmartObjects::CSmartObject::operator std::string(void) const
{
    return convert_string();
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const std::string& s)
{
    set_value_string(s);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(std::string val) const
{
    std::string comp = convert_string();
    if(comp == NsAppLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_string(const std::string& s)
{
    cleanup_data_if_type_changed_and_set_new_type(SmartType_String);
    m_data.str_value = new std::string(s);
}

std::string NsAppLink::NsSmartObjects::CSmartObject::convert_string(void) const
{
    switch (m_type) {
        case SmartType_String :
            return *(m_data.str_value);
        case SmartType_Integer:
            return std::to_string(m_data.long_value);
            break;
        case SmartType_Character:
            return std::string(1, m_data.char_value);
            break;
        case SmartType_Double:
            return std::to_string(m_data.double_value);
            break;
        default:
            break;
    }
    return invalid_string_value;
}

// =============================================================
// CHAR* TYPE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject::CSmartObject(char* s)
: m_type(SmartType_Null)
, m_schema()
{
    m_data.str_value = NULL;
    set_value_cstr(s);
    return;
}
/*
NsAppLink::NsSmartObjects::CSmartObject::operator char*(void) const
{
    return convert_string().c_str();
}
*/
NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator=(const char* s)
{
    set_value_cstr(s);
    return *this;
}

bool NsAppLink::NsSmartObjects::CSmartObject::operator==(const char* s) const
{
    std::string comp = convert_string();
    std::string val(s);
    if(comp == NsAppLink::NsSmartObjects::invalid_string_value)
    {
        return false;
    }
    else
    {
        return comp == val;
    }
}

void NsAppLink::NsSmartObjects::CSmartObject::set_value_cstr(const char* s)
{
    if (s)
        set_value_string(std::string(s));
    else {
        std::string tmp;
        set_value_string(tmp);
    }
}

// =============================================================
// ARRAY INTERFACE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](int i)
{
    return handle_array_access(i);
}

inline NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_array_access(int index)
{
    if(m_type != SmartType_Array)
    {
        cleanup_data();
        m_type = SmartType_Array;
        m_data.array_value = new SmartArray();
    }

    int sz = m_data.array_value->size();
    if (index == -1)
    {
        index = sz;
    }
    if (index == sz)
    {
        NsAppLink::NsSmartObjects::CSmartObject uc;
        m_data.array_value->push_back(uc);
    }
    if (index > sz || index < 0)
    {
        return invalid_object_value;
    }
    return m_data.array_value->at(index);
}

// =============================================================
// MAP INTERFACE SUPPORT
// =============================================================

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const std::string s)
{
    return handle_map_access(s);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](char* s)
{
    std::string str(s);
    return handle_map_access(str);
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::operator[](const char* s)
{
    std::string str(s);
    return handle_map_access(str);
}

const NsAppLink::NsSmartObjects::CSmartObject & NsAppLink::NsSmartObjects::CSmartObject::getElement(size_t Index) const
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == m_type)
    {
        if (Index < m_data.array_value->size())
        {
            return m_data.array_value->at(Index);
        }
    }

    return invalid_object_value;
}

const NsAppLink::NsSmartObjects::CSmartObject & NsAppLink::NsSmartObjects::CSmartObject::getElement(const std::string & Key) const
{
    if (NsAppLink::NsSmartObjects::SmartType_Map == m_type)
    {
        SmartMap::const_iterator i = m_data.map_value->find(Key);

        if (i != m_data.map_value->end())
        {
            return i->second;
        }
    }

    return invalid_object_value;
}

NsAppLink::NsSmartObjects::CSmartObject& NsAppLink::NsSmartObjects::CSmartObject::handle_map_access(const std::string s)
{
    if(m_type != SmartType_Map)
    {
        cleanup_data();
        m_type = SmartType_Map;
        m_data.map_value = new NsAppLink::NsSmartObjects::SmartMap();
    }

    //TODO: Add check for key presense
    return (*(m_data.map_value))[s];
}

// =============================================================
// OTHER METHODS
// =============================================================
void NsAppLink::NsSmartObjects::CSmartObject::duplicate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    SmartData newData;
    SmartType newType = object.m_type;

    switch(newType) {
        case SmartType_Map :
            newData.map_value = new SmartMap(*object.m_data.map_value);
            break;
        case SmartType_Array :
            newData.array_value = new SmartArray(*object.m_data.array_value);
            break;
        case SmartType_Integer:
            newData.long_value = object.m_data.long_value;
            break;
        case SmartType_Double :
            newData.double_value = object.m_data.double_value;
            break;
        case SmartType_Boolean :
            newData.bool_value = object.m_data.bool_value;
            break;
        case SmartType_Character :
            newData.char_value = object.m_data.char_value;
            break;
        case SmartType_String :
            newData.str_value = new std::string(*object.m_data.str_value);
            break;
            //    default : ;
    }

    cleanup_data();

    m_type = newType;
    m_data = newData;
}

void NsAppLink::NsSmartObjects::CSmartObject::cleanup_data()
{
    switch(m_type) {
    case SmartType_String :
        delete m_data.str_value;
        break;
    case SmartType_Map:
        delete m_data.map_value;
        break;
    case SmartType_Array :
        delete m_data.array_value;
        break;
//    default : ;
    }
}

size_t NsAppLink::NsSmartObjects::CSmartObject::length() const
{
    size_t size = 0;

    switch(m_type)
    {
    case SmartType_String:
        size = m_data.str_value->size();
        break;
    case SmartType_Array:
        size = m_data.array_value->size();
        break;
    case SmartType_Map:
        size = m_data.map_value->size();
        break;
    default:
        size = 0;
        break;
    }

    return size;
}

void NsAppLink::NsSmartObjects::CSmartObject::cleanup_data_if_type_changed_and_set_new_type(SmartType newType)
{
    if(m_type != newType)
    {
        cleanup_data();
        m_type = newType;
    }
}

double NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_double(const std::string* s)
{
    char* ptr;
    errno = 0;

    double result = strtod(s->c_str(),&ptr);
    if (errno ||
        (result == 0.0 && ptr != (s->c_str() + s->length())))
        return invalid_double_value;

    return result;
}

long NsAppLink::NsSmartObjects::CSmartObject::convert_string_to_long(const std::string* s)
{
    errno = 0;
    long l = strtol(s->c_str(),NULL,10);
    if (errno) return invalid_int_value;
    return l;
}

NsAppLink::NsSmartObjects::SmartType NsAppLink::NsSmartObjects::CSmartObject::get_type() const
{
    return m_type;
}

std::vector<std::string> NsAppLink::NsSmartObjects::CSmartObject::enumerate()
{
    std::vector<std::string> keys;

    if(m_type == SmartType_Map)
    {
        std::transform(
            m_data.map_value->begin(),
            m_data.map_value->end(),
            std::back_inserter(keys),
            [](const SmartMap::value_type &pair){return pair.first;}
        );
    }

    return keys;
}

bool NsAppLink::NsSmartObjects::CSmartObject::keyExists(const char * key)
{
    if(m_type != SmartType_Map)
    {
        return false;
    }

    return m_data.map_value->count(key);
}

bool NsAppLink::NsSmartObjects::CSmartObject::isValid()
{
    return m_schema.validate(*this);
}

void NsAppLink::NsSmartObjects::CSmartObject::setSchema(NsAppLink::NsSmartObjects::CSmartSchema schema)
{
    m_schema = schema;
}

NsAppLink::NsSmartObjects::CSmartSchema NsAppLink::NsSmartObjects::CSmartObject::getSchema()
{
    return m_schema;
}
