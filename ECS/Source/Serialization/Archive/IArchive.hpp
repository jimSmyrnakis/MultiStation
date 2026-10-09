#pragma once
#include <string>
/**
* @author Dimitris Smyrnakis
* @file IArchive.hpp
* @brief This file contains the Interface for the archives . These Interface's are used for
* serialization / deserialization with the implementation of the io stream remains hiden .
*/
namespace MultiStation{
	
	/**
	 * @class IArchiveWriter 
	 * @brief This Interface is used when we serialize an Component , System or a more Generic Object 
	 * . It contains methods that can be implemented for yalm , json and other formats and defines the rules 
	 * that should follow  (How to Write a hole Object - Here object means a set of fields and arrays ) .
	 */
	class IArchiveWriter {

	public:

		
		virtual ~IArchiveWriter(void) = default;

		virtual void BeginObject(const std::string& name) = 0;
		virtual void EndObject(void) = 0;

		virtual void BeginArray(const std::string& name, size_t count) = 0;
		virtual void EndArray(void) = 0;

		virtual void WriteField(const std::string& fieldName, const void* data, size_t size) = 0;
		virtual void WriteField(const std::string& fieldName, int data) = 0;
		virtual void WriteField(const std::string& fieldName, unsigned int data) = 0;
		virtual void WriteField(const std::string& fieldName, float data) = 0;
		virtual void WriteField(const std::string& fieldName, double data) = 0;
		virtual void WriteField(const std::string& fieldName, const std::string& data) = 0;

		virtual void WriteValue(const void* data, size_t size) = 0;
		virtual void WriteValue(int data) = 0;
		virtual void WriteValue(unsigned int data) = 0;
		virtual void WriteValue(float data) = 0;
		virtual void WriteValue(double data) = 0;
		virtual void WriteValue(const std::string& data) = 0;

	};

	class IArchiveReader{

	public:
		virtual ~IArchiveReader(void) = default;
		
		virtual void BeginObject(const std::string& name) = 0;
		virtual void EndObject(void) = 0;

		virtual void BeginArray(const std::string& name, size_t& count) = 0;
		virtual void EndArray(void) = 0;

		virtual void ReadField(const std::string& fieldName, void* data, size_t& size)=0;
		virtual void ReadField(const std::string& fieldName, int& data) = 0;
		virtual void ReadField(const std::string& fieldName, unsigned int& data) = 0;
		virtual void ReadField(const std::string& fieldName, float& data) = 0;
		virtual void ReadField(const std::string& fieldName, double& data) = 0;
		virtual void ReadField(const std::string& fieldName, std::string& data) = 0;

		virtual void ReadValue( void* data, size_t& size) = 0;
		virtual void ReadValue( int& data) = 0;
		virtual void ReadValue( unsigned int& data) = 0;
		virtual void ReadValue( float& data) = 0;
		virtual void ReadValue( double& data) = 0;
		virtual void ReadValue( std::string& data) = 0;

	};

}
