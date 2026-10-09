#include "TextArchive.hpp"
#include <sstream>
#include <limits>
#include <iomanip>
namespace MultiStation {

	static std::string BytesToHex(const void* data, size_t size)
	{
		static constexpr char HEX[] = "0123456789ABCDEF";

		const unsigned char* bytes =
			static_cast<const unsigned char*>(data);

		std::string result;
		result.reserve(size * 2);

		for (size_t i = 0; i < size; ++i)
		{
			result += HEX[(bytes[i] >> 4) & 0xF];
			result += HEX[bytes[i] & 0xF];
		}

		return result;
	}

	

	

	TextArchiveWriter::TextArchiveWriter(IWriteStream* stream) {
		this->stream = stream;
	}

	TextArchiveWriter::~TextArchiveWriter(void){}


	void TextArchiveWriter::BeginObject(const std::string& name) {
		std::stringstream ss;
		ss << "Object : " << name << "{ \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, const void* data, size_t size) {
		std::stringstream ss;
		ss << fieldName
			<< " = bin { "
			<< size << " , "
			<< BytesToHex(data, size)
			<< " }\n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, int data) {
		std::stringstream ss;
		ss << fieldName << " = int { " << data ;

		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, unsigned int data) {
		std::stringstream ss;
		ss << fieldName << " = uint { " << data;

		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, float data) {
		std::stringstream ss;
		ss <<   fieldName << " = float { " << std::setprecision(std::numeric_limits<float>::max_digits10) << data;

		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, double data) {
		std::stringstream ss;
		ss << fieldName << " = double { " << std::setprecision(std::numeric_limits<double>::max_digits10) << data;

		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, const std::string& data) {
		std::stringstream ss;

		ss << fieldName
			<< " = string { "
			<< std::quoted(data)
			<< " }\n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::EndObject(void) {
		std::stringstream ss;
		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::BeginArray(const std::string& name, size_t count) {
		std::stringstream ss;
		ss << "Array : " << "[" << count << " , " << name << " ] { \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}
	void TextArchiveWriter::EndArray(void) {
		std::stringstream ss;
		ss << " } \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	

	void TextArchiveWriter::WriteValue(const void* data, size_t size)  {
		std::stringstream ss;
		ss << " bin [ "
			<< size << " , "
			<< BytesToHex(data, size)
			<< " ] , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteValue(int data)  {
		std::stringstream ss;
		ss << data;

		ss << " , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteValue(unsigned int data)  {
		std::stringstream ss;
		ss << data;

		ss << " , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteValue(float data)  {
		std::stringstream ss;
		ss << std::setprecision(std::numeric_limits<float>::max_digits10) << data;

		ss << " , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteValue(double data)  {
		std::stringstream ss;
		ss << std::setprecision(std::numeric_limits<double>::max_digits10) << data;

		ss << " , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}

	void TextArchiveWriter::WriteValue(const std::string& data)  {
		std::stringstream ss;
		ss  << " = string { "
			<< std::quoted(data)
			<< " } , \n";
		stream->Write(ss.str().c_str(), ss.str().size());
	}











	// =========================================================================================================
	// ========================================================================================================
	// ========================================================================================================
	// =====================================================================================================







	static bool HexToBytes(const std::string& hex, void* data, size_t size)
	{
		if (data == nullptr)
			return false;

		if (hex.size() != size * 2)
			return false;

		auto HexValue = [](char c) -> int
			{
				if (c >= '0' && c <= '9')
					return c - '0';

				if (c >= 'A' && c <= 'F')
					return c - 'A' + 10;

				if (c >= 'a' && c <= 'f')
					return c - 'a' + 10;

				return -1;
			};

		unsigned char* bytes =
			static_cast<unsigned char*>(data);

		for (size_t i = 0; i < size; ++i)
		{
			int high = HexValue(hex[i * 2]);
			int low = HexValue(hex[i * 2 + 1]);

			if (high < 0 || low < 0)
				return false;

			bytes[i] =
				static_cast<unsigned char>((high << 4) | low);
		}

		return true;
	}

	static std::string ReadAll(IReadStream& stream)
	{
		std::string result;

		char buffer[4096];

		while (true)
		{
			size_t bytesRead = stream.Read(buffer, sizeof(buffer));

			if (bytesRead == 0)
				break;

			result.append(buffer, bytesRead);
		}

		return result;
	}


	TextArchiveReader::TextArchiveReader(IReadStream* stream) {
		this->stream = stream;
		if (stream && stream->IsOpen())
		{
			std::string text = ReadAll(*stream);
			input.str(text);
		}
	}


	TextArchiveReader::~TextArchiveReader(void){

	}


	void TextArchiveReader::BeginObject(const std::string& name) {

	}
	void TextArchiveReader::EndObject(void) {

	}

	void TextArchiveReader::BeginArray(const std::string& name, size_t& count) {

	}
	void TextArchiveReader::EndArray(void) {

	}

	void TextArchiveReader::ReadField(const std::string& fieldName, void* data, size_t& size) {

	}
	void TextArchiveReader::ReadField(const std::string& fieldName, int& data) {

	}
	void TextArchiveReader::ReadField(const std::string& fieldName, unsigned int& data) {

	}
	void TextArchiveReader::ReadField(const std::string& fieldName, float& data) {

	}
	void TextArchiveReader::ReadField(const std::string& fieldName, double& data) {

	}
	void TextArchiveReader::ReadField(const std::string& fieldName, std::string& data) {

	}

	void TextArchiveReader::ReadValue(void* data, size_t& size) {

	}
	void TextArchiveReader::ReadValue(int& data) {

	}
	void TextArchiveReader::ReadValue(unsigned int& data) {

	}
	void TextArchiveReader::ReadValue(float& data) {

	}
	void TextArchiveReader::ReadValue(double& data) {

	}
	void TextArchiveReader::ReadValue(std::string& data) {

	}

}
