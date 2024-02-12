
#include <string>
#include <map>

using ArgCode 	= std::string;
using ArgDesc 	= std::string;
using ColorCode = unsigned int;
using LangCode 	= std::string;
using TagCode 	= std::string;
using DescMap 	= std::map<LangCode, ArgDesc>;
using Priority 	= unsigned int;

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
		};

		void InitData(Data &data);
		bool FindData(const ArgCode &short_format, Data &data);
		void SetData(const ArgCode &short_format, const Data &data);
		void AppendData(const ArgCode &short_format, Data &data);

		void CopyString(const std::string &src, std::string &trg);
		void CopyUnsignedInt(const unsigned int src, unsigned int &trg);
		void CopyValidData(const Data &src_data, Data &trg_data);
		
		void CopyShortFormat(const ArgCode &src, ArgCode &trg);
		void CopyLongFormat(const ArgCode &src, ArgCode &trg);
		void CopyColorCode(const ColorCode src, ColorCode trg);
		void CopyTagCode(const TagCode &src, TagCode &trg);
		void CopyPriority(const Priority src, Priority trg);
		void CopyDescription(const DescMap &src, DescMap &trg);

		LangCode				_default_lan = "en";
		std::map<ArgCode, Data>	_args_data;

};

