
#include <string>
#include <map>

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

namespace CmdArgsCppSpace {
	extern const unsigned int invalid_code;
}


class CmdArgsCpp {

	public:
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

		LangCode				_default_lan = "en";
		std::map<ArgCode, Data>	_args_data;

};

