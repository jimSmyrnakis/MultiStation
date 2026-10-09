#pragma once
#include <fstream>
#include "Streams.hpp"
namespace MultiStation {

    class FileReadStream final : public IReadStream
    {
    public:
        explicit FileReadStream(const std::string& path)
            
        {
            this->path = path;
        }
        
        bool Open(void) override {
            if (!m_Stream.is_open())
                m_Stream.open(path, std::ios::binary);
            return m_Stream.is_open();
        }

        size_t Read(void* dest, size_t size) override
        {
            if (!m_Stream.is_open() || !dest || size == 0)
                return 0;

            m_Stream.read(
                static_cast<char*>(dest),
                static_cast<std::streamsize>(size)
            );

            return static_cast<size_t>(m_Stream.gcount());
        }

        bool IsOpen() const override
        {
            return m_Stream.is_open();
        }

    private:
        std::ifstream m_Stream;
        std::string path;
    };

    class FileWriteStream final : public IWriteStream
    {
    public:
        explicit FileWriteStream(const std::string& path)
            
        {
			this->path = path;
        }
        bool Open(void) override {
			if (!m_Stream.is_open())
				m_Stream.open(path, std::ios::binary);
			return m_Stream.is_open();
        }
        size_t Write(const void* src, size_t size) override
        {
            if (!m_Stream.is_open() || !src || size == 0)
                return 0;

            m_Stream.write(
                static_cast<const char*>(src),
                static_cast<std::streamsize>(size)
            );

            return m_Stream ? size : 0;
        }

        void Flush() override
        {
            if (m_Stream.is_open())
                m_Stream.flush();
        }

        bool IsOpen() const override
        {
            return m_Stream.is_open();
        }

    private:
        std::ofstream m_Stream;
        std::string path;
    };

}
