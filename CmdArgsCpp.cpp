#include "CmdArgsCpp.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <sstream>



using ExcpMsg = std::string;

extern const unsigned int CmdArgsCppSpace::invalid_code = -1;
static const size_t invalid_pos = -1;


namespace ExceptionMessages {

	const ExcpMsg arg_exists = "Argument already exists";
	const ExcpMsg arg_non_exists = "Argument doesn't exists";
    const ExcpMsg arg_parse_error = "Argument parsing error";
}

void CmdArgsCpp::DebugArgs()
{
    std::string tab = "\t";

    for (const auto &val : _args_data) {
        const Data &data = val.second;

        std::cout << "Argument: " << val.first << std::endl;
        std::cout << tab << "Argument long: " << data.long_format << std::endl;
        std::cout << tab << "Tag: " << data.tag << std::endl;
        std::cout << tab << "Priority: " << data.priority << std::endl;
        std::cout << tab << "Color: " << data.color << std::endl;
        std::cout << tab << "Mandatory: " << data.mandatory << std::endl;
        std::cout << tab << "NumParams: " << data.num_of_params << std::endl;
        std::cout << tab << "FormatParams: " << data.format_of_params << std::endl;

        for (const auto val2 : data.description) {
            std::cout << tab << "Language: " << val2.first << " " << "Description: " << val2.second << std::endl;
        }
    }

    std::cout << "User provided arguments" << std::endl;
    for (const auto &val : _parsed_args) {
        const StringVec &vec = val.second;

        std::cout << "Argument: " << val.first << std::endl;
        for (const auto &vec_val : vec) {
            std::cout << tab << vec_val << std::endl;
        }
    }
}


void CmdArgsCpp::InitData(Data &data)
{
    data.color = CmdArgsCppSpace::invalid_code;
    data.priority = CmdArgsCppSpace::invalid_code;
    data.mandatory = false;
    data.num_of_params = "*";       //any
    data.format_of_params = "$s";   
}

bool CmdArgsCpp::FindData(const ArgCode &short_format, Data &data)
{
    auto it = _args_data.find(short_format);
    bool ret = false;

    if (it != _args_data.end()) {
        data = it->second;
        ret = true;
    } 

    return ret;

}


void CmdArgsCpp::SetData(const ArgCode &short_format, const Data &data)
{
    auto it = _args_data.find(short_format);

    if (it != _args_data.end()) {
        it->second = data;
    }
}

void CmdArgsCpp::CopyBool(const bool src, bool &trg)
{
    trg = src;
}

void CmdArgsCpp::CopyString(const std::string &src, std::string &trg)
{
    if (src.length()) {
        if (trg.length()) {
            throw std::logic_error(ExceptionMessages::arg_exists);
        } else {
            trg = src;
        }
    }
}

void CmdArgsCpp::CopyUnsignedInt(const unsigned int src, unsigned int &trg)
{
    if (trg != CmdArgsCppSpace::invalid_code) {
        if (src != CmdArgsCppSpace::invalid_code) {
            throw std::logic_error(ExceptionMessages::arg_exists);
        } else {
            trg = src;
        }
    }
}

void CmdArgsCpp::CopyShortFormat(const ArgCode &src, ArgCode &trg)
{
    CopyString(src, trg);
}

void CmdArgsCpp::CopyLongFormat(const ArgCode &src, ArgCode &trg)
{
    CopyString(src, trg);
}

void CmdArgsCpp::CopyTagCode(const TagCode &src, TagCode &trg)
{
    CopyString(src, trg);
}

void CmdArgsCpp::CopyColorCode(const ColorCode src, ColorCode trg)
{
    CopyUnsignedInt(src, trg);
}

void CmdArgsCpp::CopyPriority(const Priority src, Priority trg)
{
    CopyUnsignedInt(src, trg);
}

void CmdArgsCpp::CopyNumOfParams(const NumParams &src, NumParams &trg)
{
    CopyString(src, trg);
}

void CmdArgsCpp::CopyFormatOfParams(const FormatParams &src, FormatParams &trg)
{
    CopyString(src, trg);
}

void CmdArgsCpp::CopyMandatory(const Mandatory src, Mandatory trg)
{
    CopyBool(src, trg);
}

void CmdArgsCpp::CopyDescription(const DescMap &src, DescMap &trg)
{
    if (src.size()) {
        for (const auto &val : src) {
            auto it = trg.find(val.first);

            if (it != trg.end()) {
                throw std::logic_error(ExceptionMessages::arg_exists);
            } else {
                trg.insert(std::make_pair(val.first, val.second));
            }
        }
    }
}



void CmdArgsCpp::CopyValidData(const Data &src, Data &trg)
{
    CopyShortFormat(src.short_format, trg.short_format);
    CopyLongFormat(src.long_format, trg.long_format);
    CopyDescription(src.description, trg.description);
    CopyColorCode(src.color, trg.color);
    CopyTagCode(src.tag, trg.tag);
    CopyPriority(src.priority, trg.priority);

}

void CmdArgsCpp::AppendData(const ArgCode &short_format, Data &data)
{
    Data old_data;

    if (FindData(short_format, old_data)) {
        CopyValidData(data, old_data);
        SetData(short_format, old_data);
    } else {
        throw std::logic_error(ExceptionMessages::arg_non_exists);
    }
}

void CmdArgsCpp::AddArgument(const ArgCode &short_format)
{
    Data data;

    if (!FindData(short_format, data)) {
        
        InitData(data);

        data.short_format = short_format;

        _args_data.insert(std::make_pair(short_format, data));
    } else {
        throw std::logic_error(ExceptionMessages::arg_exists);
    }

}

void CmdArgsCpp::AddArgument(const ArgCode &short_format, 
						     const ArgCode &long_format,
						     const ArgDesc &description, const LangCode lang)
{
    try {
        AddArgument(short_format);
        AppendLongFormat(short_format, long_format);
        AppendDescription(short_format, description, lang);
    } catch (std::logic_error e) {
        throw std::logic_error(ExceptionMessages::arg_exists);
    }
    
}

void CmdArgsCpp::AppendLongFormat(const ArgCode &short_format, const ArgCode &long_format)
{
    Data data;

    InitData(data);
    data.long_format = long_format;
    AppendData(short_format, data);
}

void CmdArgsCpp::AppendDescription(const ArgCode &short_format, const ArgDesc &description, const LangCode lang = "")
{
    LangCode language = _default_lan;

    if (!lang.empty()) {
        language = lang;
    }

    Data data;
    InitData(data);
    data.description.insert(std::make_pair(language, description));
    AppendData(short_format, data);
}

void CmdArgsCpp::AppendColor(const ArgCode &short_format, const ColorCode &color)
{
    Data data;

    InitData(data);
    data.color = color;
    AppendData(short_format, data);
}

void CmdArgsCpp::AppendTag(const ArgCode &short_format, const TagCode &tag)
{
    Data data;

    InitData(data);
    data.tag = tag;
    AppendData(short_format, data);
}


void CmdArgsCpp::AppendParamInfo(const ArgCode &short_format, 
                        const Mandatory mandatory, 
                        const NumParams &num_of_params, 
                        const FormatParams &format_of_params)
{
    Data data;

    InitData(data);
    data.mandatory = mandatory;
    data.num_of_params = num_of_params;
    data.format_of_params = format_of_params;
    AppendData(short_format, data);
}

std::size_t CmdArgsCpp::FindArgumentsInStringSub(const std::string &cmd_args, const ArgCode &arg)
{
    size_t pos = cmd_args.find(arg);

    return pos;
}

void CmdArgsCpp::FindArgumentInString(const std::string &cmd_args, const Data &data, 
                                            PosDataMap &pos_data_map)
{
    std::size_t pos = std::string::npos;

    pos = FindArgumentsInStringSub(cmd_args, "-" + data.short_format);

    if (pos == std::string::npos) { 
        pos = FindArgumentsInStringSub(cmd_args, "--" + data.long_format);
    }

    if (pos != std::string::npos) {
        pos_data_map.insert(std::make_pair(pos, data));
    }

}

void CmdArgsCpp::Tokenize(const std::string str, const char delim, StringVec &vec)
{
    std::stringstream ss(str);
    std::string token;
         
    while(getline(ss, token, delim)) {
        vec.push_back(token);
    }
     
}

void CmdArgsCpp::ExtractCmdArgInfo(const std::string &cmd_arg, const Data &data)
{

    std::size_t pos = cmd_arg.find(data.short_format);

    if (pos == std::string::npos) {
        pos = cmd_arg.find(data.long_format);
    }

    if (pos != std::string::npos) {        
        std::string only_arg_value = "";

        if (pos + 1 < cmd_arg.length()) {
            only_arg_value = cmd_arg.substr(pos + 1);
        }

        StringVec vec;

        if (only_arg_value.size()) {
            Tokenize(only_arg_value, ' ', vec);
        }
        
        _parsed_args.insert(std::make_pair(data.short_format, vec));

    } else {
        assert(0);
    }

}

void CmdArgsCpp::StoreArgumentInfo(const std::string &cmd_args, const PosDataMap &pos_data_map)
{
    std::size_t size = pos_data_map.size();

    if (!size) {
        return;
    }

    for (auto it = pos_data_map.begin(); it != pos_data_map.end(); ++it) {
        std::size_t pos1 = it->first;
        std::size_t pos2 = std::string::npos;

        auto it2 = it;
        if (++it2 != pos_data_map.end()) {
            pos2 = it2->first; 
        } 

        std::string cmd_arg = cmd_args.substr(pos1, pos2 - pos1);
        ExtractCmdArgInfo(cmd_arg, it->second);
    }

}

void CmdArgsCpp::ParseCmdArguments(std::string &cmd_args)
{
    PosDataMap pos_data_map;

    auto func = [this, &cmd_args, &pos_data_map](ArgDataPair pair_v) {
            
            FindArgumentInString(cmd_args, pair_v.second, pos_data_map);
    };

    std::for_each(_args_data.begin(), _args_data.end(), func);
    
    StoreArgumentInfo(cmd_args, pos_data_map);
}

void CmdArgsCpp::ParseArguments(const int argc, const char *const argv[])
{
    if (argv[argc] != nullptr) {
        throw std::logic_error(ExceptionMessages::arg_parse_error);
    } else if (argc != 1) {
        std::string cmd_args;

        for (int i = 1; i < argc; ++i) {
            cmd_args.append(argv[i]);
        }

        ParseCmdArguments(cmd_args);
    }
}
