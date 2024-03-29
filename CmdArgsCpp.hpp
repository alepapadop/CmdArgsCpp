#ifndef CMD_ARGS_CPP_HPP
#define CMD_ARGS_CPP_HPP


#include <string>
#include <map>
#include <vector>




namespace CmdArgs {
	using ArgCode 		= std::string;
	using ArgDesc 		= std::string;
	using ColorCode 	= unsigned int;
	using LangCode 		= std::string;
	using TagCode 		= std::string;
	using DescMap 		= std::map<LangCode, ArgDesc>;
	using Priority 		= unsigned int;
	using NumParams 	= std::string;
	using FormatParams 	= std::string;
	using Mandatory 	= bool;
	using StringVec		= std::vector<std::string>;
	using ArgMap		= std::map<ArgCode, StringVec>;

	namespace CmdArgsCppSpace {
		extern const unsigned int invalid_code;

		const NumParams any_param = "*";
		const NumParams at_least_postfix = "+";
		const NumParams max_postfix = "-";

		const FormatParams str = "string";
		const FormatParams integer = "integer";
		const FormatParams decimal = "float";
		const FormatParams alpha = "alpha";
	}


	class CmdArgsCpp {

		public:
			
			enum ArgType {
				ARG_TYPE_INT,
				ARG_TYPE_DOUBLE,
				ARG_TYPE_STR,
				ARG_TYPE_ALPHA
			};

			enum ErrorCode {
				
			};

			CmdArgsCpp() = default;

			void AddArgument(const ArgCode &short_format);

			void AddArgument(const ArgCode &short_format, 
							const ArgCode &long_format,
							const ArgDesc &description, const LangCode lang);

			void AppendLongFormat(const ArgCode &short_format, const ArgCode &long_format);
			void AppendDescription(const ArgCode &short_format, const ArgDesc &description, const LangCode lang);
			void AppendColor(const ArgCode &short_format, const ColorCode &color);
			void AppendTag(const ArgCode &short_format, const TagCode &tag);
			void AppendParamInfo(const ArgCode &short_format, 
									const Mandatory mandatory, 
									const NumParams &num_of_params, 
									const FormatParams &format_of_params);
				
			void ParseJsonFileForArguments(std::string &file_path);

			void ParseArguments(const int argc, const char *const argv[]);
			bool CheckUserArguments();
			void GetUserArgument(const ArgCode &short_format);
			void PrintArgumentHelp();
			

			void DebugArgs();

		private:
			

			struct Data {
				ArgCode 		short_format;
				ArgCode			long_format;

				DescMap			description;

				ColorCode		color;
				TagCode			tag;
				Priority		priority;

				bool			mandatory;
				std::string		num_of_params;
				std::string		format_of_params;
			};

			using ArgDataPair = std::pair<ArgCode, Data>;
			using PosDataMap = std::map<size_t, Data>;
			using ArgData = std::map<ArgCode, Data>;
			
			

			void InitData(Data &data);
			bool FindData(const ArgCode &short_format, Data &data);
			void SetData(const ArgCode &short_format, const Data &data);
			void AppendData(const ArgCode &short_format, Data &data);

			void CopyBool(const bool src, bool &trg);
			void CopyString(const std::string &src, std::string &trg);
			void CopyUnsignedInt(const unsigned int src, unsigned int &trg);
			void CopyValidData(const Data &src_data, Data &trg_data);
			
			void CopyShortFormat(const ArgCode &src, ArgCode &trg);
			void CopyLongFormat(const ArgCode &src, ArgCode &trg);
			void CopyColorCode(const ColorCode src, ColorCode trg);
			void CopyTagCode(const TagCode &src, TagCode &trg);
			void CopyPriority(const Priority src, Priority trg);
			void CopyDescription(const DescMap &src, DescMap &trg);
			void CopyNumOfParams(const NumParams &src, NumParams &trg);
			void CopyFormatOfParams(const FormatParams &src, FormatParams &trg);
			void CopyMandatory(const Mandatory src, Mandatory trg);
			bool ValidateNumOfParams(const std::string &num_params);
			bool ValidateFormatOfParams(const std::string &num_params);

			void ParseCmdArguments(std::string &cmd_args);		
			void FindArgumentInString(const std::string &cmd_args, const Data &data, 
											PosDataMap &short_format_pos_map);
			size_t FindArgumentsInStringSub(const std::string &cmd_args, const ArgCode &arg);
			void StoreArgumentInfo(const std::string &cmd_args, const PosDataMap &pos_data_map);
			void ExtractCmdArgInfo(const std::string &cmd_arg, const Data &data);
			void Tokenize(const std::string str, const char delim, StringVec &vec);
			bool CheckUserArgument(const std::string &short_format, const Data &data, const StringVec &vec);
			bool CheckUserArgumentNumParam(const Data &data, const StringVec &vec);
			bool CheckUserArgumentFormatParam(const Data &data, const StringVec &vec);
			unsigned int ExtractIntFromString(const std::string &s, const std::string &allowed_char);
			bool StringContainsOnlyCharacters(const std::string &str);
			bool StringContainsInteger(const std::string &str);
			bool StringContainsFloat(const std::string &str);

			LangCode		_default_lan = "en";
			ArgData			_args_data;
			ArgMap			_parsed_args;

	};
}

#endif

