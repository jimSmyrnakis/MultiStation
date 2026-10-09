#pragma once
#include "../../Streams/Streams.hpp"
#include "IArchive.hpp"
#include <sstream>
namespace MultiStation {

	class TextArchiveReader : public IArchiveReader {
	public:

		TextArchiveReader(IReadStream* stream);

		~TextArchiveReader(void);


		void BeginObject(const std::string& name) override;
		void EndObject(void) override;

		void BeginArray(const std::string& name, size_t& count) override;
		void EndArray(void) override;

		void ReadField(const std::string& fieldName, void* data, size_t& size) override;
		void ReadField(const std::string& fieldName, int& data) override;
		void ReadField(const std::string& fieldName, unsigned int& data) override;
		void ReadField(const std::string& fieldName, float& data) override;
		void ReadField(const std::string& fieldName, double& data) override;
		void ReadField(const std::string& fieldName, std::string& data) override;

		void ReadValue(void* data, size_t& size) override;
		void ReadValue(int& data) override;
		void ReadValue(unsigned int& data) override;
		void ReadValue(float& data) override;
		void ReadValue(double& data) override;
		void ReadValue(std::string& data) override;

	private:

		IReadStream* stream;
		std::istringstream input;
	};


	class TextArchiveWriter : public IArchiveWriter {
	public:

		TextArchiveWriter(IWriteStream* stream);

		~TextArchiveWriter(void);


		void BeginObject(const std::string& name) override;
		void EndObject(void) override;

		void BeginArray(const std::string& name, size_t count) override;
		void EndArray(void) override;

		void WriteField(const std::string& fieldName, const void* data, size_t size) override;
		void WriteField(const std::string& fieldName, int data) override;
		void WriteField(const std::string& fieldName, unsigned int data) override;
		void WriteField(const std::string& fieldName, float data) override;
		void WriteField(const std::string& fieldName, double data) override;
		void WriteField(const std::string& fieldName, const std::string& data) override;

		void WriteValue(const void* data, size_t size) override;
		void WriteValue(int data) override;
		void WriteValue(unsigned int data) override;
		void WriteValue(float data) override;
		void WriteValue(double data) override;
		void WriteValue(const std::string& data) override;	
	private:

		IWriteStream* stream;

	};

}
