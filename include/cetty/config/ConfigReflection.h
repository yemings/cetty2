#if !defined(CETTY_CONFIG_CONFIGREFLECTION_H)
#define CETTY_CONFIG_CONFIGREFLECTION_H

/*
 * Copyright (c) 2010-2012 frankee zhou (frankee.zhou at gmail dot com)
 *
 * Distributed under under the Apache License, version 2.0 (the "License").
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <string>
#include <vector>
#include <boost/cstdint.hpp>
#include <cetty/config/ConfigDescriptor.h>

namespace cetty {
namespace config {

class ConfigObject;
class ConfigFieldDescriptor;

class ConfigReflection {
public:
    ConfigReflection();

    void ListFields(const ConfigObject& object,
                    std::vector<const ConfigFieldDescriptor*>* output) const;

    int getInt(const ConfigObject& object,
               const ConfigFieldDescriptor* field) const;

    boost::int64_t getInt64(const ConfigObject& object,
                            const ConfigFieldDescriptor* field) const;
    double getDouble(const ConfigObject& object,
                     const ConfigFieldDescriptor* field) const;
    bool getBool(const ConfigObject& object,
                 const ConfigFieldDescriptor* field) const;
    std::string getString(const ConfigObject& object,
                          const ConfigFieldDescriptor* field) const;

    void setInt32(ConfigObject* object,
                  const ConfigFieldDescriptor* field,
                  int value) const {
        setField<int>(object, field, value);
    }

    void setInt64(ConfigObject* object,
                  const ConfigFieldDescriptor* field,
                  boost::int64_t value) const {
        setField<boost::int64_t>(object, field, value);
    }
    void setDouble(ConfigObject* object,
                   const ConfigFieldDescriptor* field,
                   double value) const {
        setField<double>(object, field, value);
    }
    void setBool(ConfigObject* object,
                 const ConfigFieldDescriptor* field,
                 bool value) const {
        setField<bool>(object, field, value);
    }
    void setString(ConfigObject* object,
                   const ConfigFieldDescriptor* field,
                   const std::string& value) const {
        setField<std::string>(object, field, value);
    }

    ConfigObject* MutableMessage(ConfigObject* object,
                                 const ConfigFieldDescriptor* field) const {
        return mutableRaw<ConfigObject>(object, field);
    }

    void addInt32(ConfigObject* object,
                  const ConfigFieldDescriptor* field,
                  int value) const {
        addField<int>(object, field, value);
    }
    void addInt64(ConfigObject* object,
                  const ConfigFieldDescriptor* field,
                  boost::int64_t value) const {
        addField<boost::int64_t>(object, field, value);
    }
    void addDouble(ConfigObject* object,
                   const ConfigFieldDescriptor* field,
                   double value) const {
        addField<double>(object, field, value);
    }
    void addBool(ConfigObject* object,
                 const ConfigFieldDescriptor* field,
                 bool value) const {
        addField<bool>(object, field, value);
    }
    void addString(ConfigObject* object,
                   const ConfigFieldDescriptor* field,
                   const std::string& value) const {
        addField<std::string>(object, field, value);
    }

    ConfigObject* addMessage(ConfigObject* object, const ConfigFieldDescriptor* field) const;

private:
    template <typename Type>
    inline void setField(ConfigObject* object,
                         const ConfigFieldDescriptor* field,
                         const Type& value) const {
        *mutableRaw<Type>(object, field) = value;
    }

    template <typename Type>
    inline Type* mutableRaw(ConfigObject* object,
                            const ConfigFieldDescriptor* field) const {
        void* ptr = reinterpret_cast<boost::uint8_t*>(object) + field->offset;
        return reinterpret_cast<Type*>(ptr);
    }

    template <typename Type>
    inline void addField(ConfigObject* object,
                         const ConfigFieldDescriptor* field,
                         const Type& value) const {
        mutableRaw<std::vector<Type> >(object, field)->push_back(value);
    }
};

}
}

#endif //#if !defined(CETTY_CONFIG_CONFIGREFLECTION_H)

// Local Variables:
// mode: c++
// End: