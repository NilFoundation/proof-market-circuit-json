//---------------------------------------------------------------------------//
// Copyright (c) 2022 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2022 Aleksei Moskvin <alalmoskvin@nil.foundation>
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------//

#ifndef CIRCUIT_JSON_SERIALIZATION_HPP
#define CIRCUIT_JSON_SERIALIZATION_HPP

#include <vector>
#include <boost/json/src.hpp>
#include <nil/crypto3/circuit_json/structs_serialization.hpp>

namespace nil {
    namespace crypto3 {
        namespace circuit_json {

            void pretty_print(std::ostream &os, boost::json::value const &jv, std::string *indent = nullptr) {
                std::string indent_;
                if (!indent) {
                    indent = &indent_;
                }
                switch (jv.kind()) {
                    case boost::json::kind::object: {
                        os << "{\n";
                        indent->append(4, ' ');
                        auto const &obj = jv.get_object();
                        if (!obj.empty()) {
                            auto it = obj.begin();
                            for (;;) {
                                os << *indent << boost::json::serialize(it->key()) << " : ";
                                pretty_print(os, it->value(), indent);
                                if (++it == obj.end())
                                    break;
                                os << ",\n";
                            }
                        }
                        os << "\n";
                        indent->resize(indent->size() - 4);
                        os << *indent << "}";
                        break;
                    }

                    case boost::json::kind::array: {
                        os << "[\n";
                        indent->append(4, ' ');
                        auto const &arr = jv.get_array();
                        if (!arr.empty()) {
                            auto it = arr.begin();
                            for (;;) {
                                os << *indent;
                                pretty_print(os, *it, indent);
                                if (++it == arr.end())
                                    break;
                                os << ",\n";
                            }
                        }
                        os << "\n";
                        indent->resize(indent->size() - 4);
                        os << *indent << "]";
                        break;
                    }

                    case boost::json::kind::string: {
                        os << boost::json::serialize(jv.get_string());
                        break;
                    }

                    case boost::json::kind::uint64:
                        os << jv.get_uint64();
                        break;

                    case boost::json::kind::int64:
                        os << jv.get_int64();
                        break;

                    case boost::json::kind::double_:
                        os << jv.get_double();
                        break;

                    case boost::json::kind::bool_:
                        if (jv.get_bool())
                            os << "true";
                        else
                            os << "false";
                        break;

                    case boost::json::kind::null:
                        os << "null";
                        break;
                }

                if (indent->empty())
                    os << "\n";
            }

            template <typename BlueprintFieldType, typename ArithmetizationParams>
            void serialize(std::ostream &os, zk::snark::plonk_table_description<BlueprintFieldType, ArithmetizationParams> desc,
                           zk::blueprint_public_assignment_table<zk::snark::plonk_constraint_system<BlueprintFieldType, ArithmetizationParams>> public_assignment,
                           zk::blueprint<zk::snark::plonk_constraint_system<BlueprintFieldType, ArithmetizationParams>> bp) {
                boost::json::value jv = {
                        {"curve_type", "pallas"},
                        {"hash", "keccak_1600<256>"},
                        {"public_assignment",         public_assignment},
                        {"desc",                      desc},
                        {"bp",                        bp},
                };
                circuit_json::pretty_print(os, jv);
            }

        }    // namespace math
    }        // namespace crypto3
}    // namespace nil

#endif    // CIRCUIT_JSON_SERIALIZATION_HPP
