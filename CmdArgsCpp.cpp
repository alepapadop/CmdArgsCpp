#include "CmdArgsCpp.hpp"
#include <iostream>
#include <algorithm>



using ExcpMsg = std::string;

extern const unsigned int CmdArgsCppSpace::invalid_code = -1;


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

size_t CmdArgsCpp::FindArgumentsInStringSub(const std::string &cmd_args, const ArgCode &arg)
{
    size_t pos = cmd_args.find(arg);

    if (pos == std::string::npos) {
        pos = 0;
    }

    return pos;
}

size_t CmdArgsCpp::FindArgumentInString(const std::string &cmd_args, Data &data)
{
    size_t pos;

    pos = FindArgumentsInStringSub(cmd_args, "-" + data.short_format);

    if (!pos) { 
        pos = FindArgumentsInStringSub(cmd_args, "--" + data.long_format);
    }

    return pos;

}

void CmdArgsCpp::StoreArgumentInfo(const std::string &cmd_args, const size_t pos, Data &data)
{
    if (!pos) {
        return;
    }

    // kati me union edw kai vector
}

void CmdArgsCpp::ParseCmdArguments(std::string &cmd_args)
{

    auto func = [this, &cmd_args](ArgDataPair pair_v) {
            size_t pos = FindArgumentInString(cmd_args, pair_v.second);
            StoreArgumentInfo(cmd_args, pos, pair_v.second);

    };

    std::for_each(_args_data.begin(), _args_data.end(), nullptr);
    
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


        
    }
}
