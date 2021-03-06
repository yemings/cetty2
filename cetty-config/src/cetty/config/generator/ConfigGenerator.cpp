
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

#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/command_line_interface.h>

#include <cetty/config/generator/cpp/CppGenerator.h>
#include <cetty/config/generator/yaml/YamlGenerator.h>

using namespace google::protobuf::compiler;
using namespace cetty::config::generator::cpp;
using namespace cetty::config::generator::yaml;

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    CppGenerator cppGenerator;
    YamlGenerator yamlGenerator;

    google::protobuf::compiler::CommandLineInterface cli;
    cli.RegisterGenerator("--config_out", &cppGenerator,
        "Generate C++ header and source.");

    cli.RegisterGenerator("--yaml_out", &yamlGenerator,
        "Generate yaml configure file.");

    return cli.Run(argc, argv);

    //PluginMain(argc, argv, &generator);
}

