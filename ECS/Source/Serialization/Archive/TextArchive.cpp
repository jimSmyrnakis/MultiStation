#include "TextArchive.hpp"
#include <sstream>
#include <limits>
#include <iomanip>
#include <Platform.hpp>
namespace MultiStation {

	

	TextArchiveWriter::~TextArchiveWriter(void){}

	void* TextArchiveWriter::GetBuffer(void) {
		m_bufferCache = m_output.str();
		return m_bufferCache.data();
	}

	uint32_t TextArchiveWriter::GetCount(void) {
		return m_output.str().length();
	}

	void TextArchiveWriter::ClearBuffer(void) {
		m_output.str("");
		m_output.clear();
		m_bufferCache.clear();
	}

	void TextArchiveWriter::BeginObject(const std::string& name) {
		m_output << name << " { " << std::endl;
		
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, const void* data, size_t size) {
		m_output << fieldName << " : ( " << size << " , ";
		m_output.write((const char*) data , size);
		m_output << " ) " << std::endl;
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, int data) {
		m_output << fieldName << " : " << data << std::endl;
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, unsigned int data) {
		m_output << fieldName << " : " << data << std::endl;
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, float data) {
		m_output << fieldName << " : " << data << std::endl;
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, double data) {
		m_output << fieldName << " : " <<  data << std::endl;
	}

	void TextArchiveWriter::WriteField(const std::string& fieldName, const std::string& data) {
		m_output << fieldName << " : ( " << data.length() << " , ";
		m_output.write(data.c_str(), data.length());
		m_output << " ) " << std::endl;
	}

	void TextArchiveWriter::EndObject(void) {
		m_output << " } " << std::endl;
	}

	// =========================================================================================================
	// ========================================================================================================
	// ========================================================================================================
	// =====================================================================================================



	TextArchiveReader::~TextArchiveReader(void) {

	}

	void TextArchiveReader::SetBuffer(void* buffer, size_t count) {
		m_input.str(std::string(
			static_cast<const char*>(buffer), count
		));
		m_input.clear();
	}

	void TextArchiveReader::BeginObject(const std::string& name) {
		std::ostringstream obj;
		obj << name << " { " << std::endl;
		size_t index = m_input.str().find(obj.str(), m_input.tellg());
		m_input.seekg(index + obj.str().length(), std::ios_base::beg);

	}
	void TextArchiveReader::EndObject(void) {
		
	}



	void TextArchiveReader::ReadField(const std::string& fieldName, void** data, size_t& size) {
		std::ostringstream obj;
		obj << fieldName << " : ( ";

		size_t index = m_input.str().find(obj.str() , m_input.tellg());
		m_input.seekg(index + obj.str().length(), std::ios_base::beg);

		m_input >> size;

		m_input.ignore(3); // Skip " , "

		(*data) = malloc(size);
		m_input.read((char*)*data, size);
	}
	void TextArchiveReader::ReadField(const std::string& fieldName, int& data) {
		std::ostringstream obj;
		obj << fieldName << " : ";

		size_t index = m_input.str().find(obj.str(), m_input.tellg());

		m_input.seekg(index + obj.str().length(), std::ios_base::beg);
		m_input >> data;
	}
	void TextArchiveReader::ReadField(const std::string& fieldName, unsigned int& data) {
		std::ostringstream obj;
		obj << fieldName << " : ";

		size_t index = m_input.str().find(obj.str(), m_input.tellg());

		m_input.seekg(index + obj.str().length(), std::ios_base::beg);
		m_input >> data;
	}
	void TextArchiveReader::ReadField(const std::string& fieldName, float& data) {
		std::ostringstream obj;
		obj << fieldName << " : ";

		size_t index = m_input.str().find(obj.str(), m_input.tellg());

		m_input.seekg(index + obj.str().length(), std::ios_base::beg);
		m_input >> data;
	}
	void TextArchiveReader::ReadField(const std::string& fieldName, double& data) {
		std::ostringstream obj;
		obj << fieldName << " : ";

		size_t index = m_input.str().find(obj.str(), m_input.tellg());

		m_input.seekg(index + obj.str().length(), std::ios_base::beg);
		m_input >> data;
	}
	void TextArchiveReader::ReadField(
		const std::string& fieldName,
		std::string& data
	) {
		std::ostringstream obj;
		obj << fieldName << " : ( ";

		size_t index = m_input.str().find(obj.str(), m_input.tellg());
		m_input.seekg(index + obj.str().length(), std::ios_base::beg);

		size_t len = 0;
		m_input >> len;

		m_input.ignore(3); // Skip " , "

		data.resize(len);
		m_input.read(data.data(), len);
	}







































	void TextArchiveWriter::BeginArray(const std::string& name, size_t count) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveWriter::EndArray(void) {
		MS_ASSERT(false, "Not implemented !");
	}

	

	void TextArchiveWriter::WriteValue(const void* data, size_t size)  {
		MS_ASSERT(false, "Not implemented !");
	}

	void TextArchiveWriter::WriteValue(int data)  {
		MS_ASSERT(false, "Not implemented !");
	}

	void TextArchiveWriter::WriteValue(unsigned int data)  {
		MS_ASSERT(false, "Not implemented !");
	}

	void TextArchiveWriter::WriteValue(float data)  {
		MS_ASSERT(false, "Not implemented !");
	}

	void TextArchiveWriter::WriteValue(double data)  {
		MS_ASSERT(false, "Not implemented !");
	}

	void TextArchiveWriter::WriteValue(const std::string& data)  {
		MS_ASSERT(false, "Not implemented !");
	}











	

	void TextArchiveReader::BeginArray(const std::string& name, size_t& count) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::EndArray(void) {
		MS_ASSERT(false, "Not implemented !");
	}

	

	void TextArchiveReader::ReadValue(void* data, size_t& size) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::ReadValue(int& data) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::ReadValue(unsigned int& data) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::ReadValue(float& data) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::ReadValue(double& data) {
		MS_ASSERT(false, "Not implemented !");
	}
	void TextArchiveReader::ReadValue(std::string& data) {
		MS_ASSERT(false, "Not implemented !");
	}

}
