#pragma once

#include "mspch.h"
#include <Application.hpp>
#include <IO.hpp>

#include <algorithm>
#include <array>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace MultiStation
{
    class ExamplePanel : public IDrawable
    {
    public:
        virtual inline void Draw(void) override
        {
            if (!m_initialized)
            {
                m_initialized = true;

                m_heavyWriteBuffer.resize(1024ull * 1024ull);

                static constexpr char pattern[] =
                    "MultiStation Async IO Stress Test\n";

                for (size_t i = 0; i < m_heavyWriteBuffer.size(); ++i)
                {
                    m_heavyWriteBuffer[i] =
                        pattern[i % (sizeof(pattern) - 1)];
                }

                m_slowReadBuffer.resize(m_slowReadChunkSize + 1);
                m_asyncReadBuffer.resize(4096);

                SetDirectory(".");
                AddLog("IO test panel initialized.");
            }

            UpdateSlowReader();

            ImGui::Begin("IO Test Bench");

            DrawOverview();

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "1. Directory Browser",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawDirectoryBrowser();
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "2. Synchronous IO Suite",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawSyncSuite();
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "3. Asynchronous IO Chain",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawAsyncChain();
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "4. Async Burst Queue Test",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawBurstTest();
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "5. Heavy Async Writer",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawHeavyWriter();
            }

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "6. Slow Async Reader",
                ImGuiTreeNodeFlags_DefaultOpen))
            {
                DrawSlowReader();
            }

            ImGui::Separator();

            DrawLog();

            ImGui::End();
        }

    private:
        // =========================================================
        // COMMON UI / LOGGING
        // =========================================================

        inline void AddLog(const std::string& text)
        {
            constexpr size_t MAX_LOG_LINES = 300;

            if (m_log.size() >= MAX_LOG_LINES)
                m_log.erase(m_log.begin());

            m_log.push_back(text);
            MS_INFO("%s", text.c_str());
        }

        inline void AddError(const std::string& text)
        {
            constexpr size_t MAX_LOG_LINES = 300;

            if (m_log.size() >= MAX_LOG_LINES)
                m_log.erase(m_log.begin());

            m_log.push_back("[ERROR] " + text);
            MS_ERROR("%s", text.c_str());
        }

        inline static bool Completed(const IO::Result& result)
        {
            return result.status == IO::Status::COMPLETED;
        }

        inline void DrawOverview()
        {
            ImGui::TextUnformatted(
                "This panel exercises only the IO subsystem.");

            ImGui::TextUnformatted(
                "It covers sync calls, async chains, queue bursts, "
                "directory IO, file info, seek/tell, read/write and close.");

            ImGui::TextDisabled(
                "Important: IO::PollCallbacks() must run from the main loop.");
        }

        inline void DrawLog()
        {
            ImGui::TextUnformatted("IO Log");

            if (ImGui::Button("Clear Log"))
                m_log.clear();

            ImGui::BeginChild(
                "##IOLog",
                ImVec2(0.0f, 220.0f),
                true,
                ImGuiWindowFlags_HorizontalScrollbar);

            for (const std::string& line : m_log)
                ImGui::TextUnformatted(line.c_str());

            ImGui::EndChild();
        }

        // =========================================================
        // DIRECTORY BROWSER
        // =========================================================

        inline void DrawDirectoryBrowser()
        {
            if (m_directoryLoading)
                ImGui::BeginDisabled();

            if (ImGui::Button("Up"))
                GoUp();

            ImGui::SameLine();

            if (ImGui::Button("Refresh"))
                RefreshDirectory();

            ImGui::SameLine();

            ImGui::SetNextItemWidth(-1.0f);

            const bool enter =
                ImGui::InputText(
                    "##DirectoryPath",
                    m_pathBuffer,
                    sizeof(m_pathBuffer),
                    ImGuiInputTextFlags_EnterReturnsTrue);

            if (enter)
                SetDirectory(m_pathBuffer);

            if (m_directoryLoading)
                ImGui::EndDisabled();

            ImGui::Text(
                "Current Directory: %s",
                m_currentPath.c_str());

            ImGui::Text(
                "Entries: %zu",
                m_entries.size());

            if (m_directoryLoading)
                ImGui::TextDisabled("ReadDirAsync pending...");

            if (ImGui::BeginChild(
                "##DirectoryEntries",
                ImVec2(0.0f, 260.0f),
                true))
            {
                for (size_t i = 0; i < m_entries.size(); ++i)
                {
                    const IO::DirectoryEntry& entry = m_entries[i];

                    ImGui::PushID(static_cast<int>(i));

                    const std::string label =
                        std::string(entry.is_directory ? "[DIR] " : "[FILE] ") +
                        entry.name;

                    const bool selected =
                        m_selectedIndex == static_cast<int>(i);

                    if (ImGui::Selectable(
                        label.c_str(),
                        selected,
                        ImGuiSelectableFlags_AllowDoubleClick))
                    {
                        m_selectedIndex = static_cast<int>(i);

                        if (ImGui::IsMouseDoubleClicked(
                            ImGuiMouseButton_Left))
                        {
                            const std::string path = entry.path;
                            const bool isDirectory = entry.is_directory;

                            ImGui::PopID();

                            if (isDirectory)
                                SetDirectory(path);
                            else
                                StartSlowRead(path);

                            break;
                        }
                    }

                    ImGui::PopID();
                }
            }

            ImGui::EndChild();

            if (m_selectedIndex < 0 ||
                m_selectedIndex >= static_cast<int>(m_entries.size()))
            {
                ImGui::TextDisabled("No entry selected.");
                return;
            }

            const IO::DirectoryEntry& entry =
                m_entries[static_cast<size_t>(m_selectedIndex)];

            ImGui::Text("Name: %s", entry.name.c_str());
            ImGui::TextWrapped("Path: %s", entry.path.c_str());

            if (entry.is_directory)
            {
                ImGui::TextUnformatted("Type: Directory");

                if (ImGui::Button("Open Selected Directory"))
                    SetDirectory(entry.path);
            }
            else
            {
                ImGui::TextUnformatted("Type: File");
                ImGui::Text("Size: %zu bytes", entry.size);

                if (m_slowReading)
                    ImGui::BeginDisabled();

                if (ImGui::Button("Open Selected In Slow Reader"))
                    StartSlowRead(entry.path);

                if (m_slowReading)
                    ImGui::EndDisabled();
            }
        }

        inline bool SetDirectory(const std::string& path)
        {
            if (m_directoryLoading)
                return false;

            m_directoryLoading = true;

            const std::string requestedPath = path;

            const bool accepted =
                IO::ReadDirAsync(
                    requestedPath,
                    [this, requestedPath](IO::Result& result)
                    {
                        m_directoryLoading = false;

                        if (!Completed(result))
                        {
                            AddError(
                                "ReadDirAsync failed for: " +
                                requestedPath +
                                " error=" +
                                std::to_string(
                                    static_cast<int>(result.error)));
                            return;
                        }

                        std::vector<IO::DirectoryEntry> entries =
                            std::move(result.entries);

                        std::sort(
                            entries.begin(),
                            entries.end(),
                            [](const IO::DirectoryEntry& a,
                                const IO::DirectoryEntry& b)
                            {
                                if (a.is_directory != b.is_directory)
                                    return a.is_directory > b.is_directory;

                                return a.name < b.name;
                            });

                        m_currentPath = requestedPath;
                        m_entries = std::move(entries);
                        m_selectedIndex = -1;

                        std::snprintf(
                            m_pathBuffer,
                            sizeof(m_pathBuffer),
                            "%s",
                            m_currentPath.c_str());

                        AddLog(
                            "ReadDirAsync completed: " +
                            m_currentPath +
                            " entries=" +
                            std::to_string(m_entries.size()));
                    });

            if (!accepted)
            {
                m_directoryLoading = false;
                AddError(
                    "ReadDirAsync request rejected for: " +
                    requestedPath);
                return false;
            }

            AddLog(
                "ReadDirAsync queued: " +
                requestedPath);

            return true;
        }

        inline void RefreshDirectory()
        {
            if (!m_directoryLoading)
                SetDirectory(m_currentPath);
        }

        inline void GoUp()
        {
            if (m_directoryLoading)
                return;

            std::filesystem::path path(m_currentPath);

            if (path.empty() || path == ".")
                path = std::filesystem::absolute(path);

            const std::filesystem::path parent =
                path.parent_path();

            if (!parent.empty())
                SetDirectory(parent.string());
        }

        // =========================================================
        // SYNCHRONOUS SUITE
        // =========================================================

        inline void DrawSyncSuite()
        {
            ImGui::TextUnformatted(
                "Sequence: CreateDir -> CreateFile -> Write -> Tell -> "
                "Seek -> Read -> GetInfo(handle) -> Close -> "
                "GetInfo(path) -> ReadDir -> RemoveFile -> RemoveDir");

            if (ImGui::Button("Run Full Sync Suite"))
                RunSyncSuite();

            if (!m_syncStatus.empty())
                ImGui::TextWrapped("%s", m_syncStatus.c_str());
        }

        inline void RunSyncSuite()
        {
            ++m_testRunId;

            const std::string dir =
                "./io_sync_suite_" +
                std::to_string(m_testRunId);

            const std::string file =
                dir + "/sync_test.txt";

            const std::string payload =
                "MultiStation synchronous IO suite\n"
                "write -> tell -> seek -> read -> info -> close\n";

            AddLog("===== SYNC SUITE START =====");

            if (!IO::CreateDirSync(dir))
            {
                m_syncStatus = "CreateDirSync failed.";
                AddError(m_syncStatus + " " + dir);
                return;
            }

            AddLog("CreateDirSync completed: " + dir);

            std::shared_ptr<IO::Handle> handle =
                IO::CreateFileSync(
                    file,
                    IO::Access::READ | IO::Access::WRITE);

            if (!handle)
            {
                m_syncStatus = "CreateFileSync failed.";
                AddError(m_syncStatus + " " + file);
                IO::RemoveDirSync(dir);
                return;
            }

            AddLog("CreateFileSync completed: " + file);

            size_t write_bytes = payload.size();

            size_t& written = write_bytes;
                IO::WriteFileSync(
                    handle,
                    payload.data(),
                    write_bytes);

            AddLog(
                "WriteFileSync transferred=" +
                std::to_string(written));

            size_t writePosition;
                IO::TellFileSync(handle , writePosition);

            AddLog(
                "TellFileSync position=" +
                std::to_string(writePosition));

            IO::FileInfo handleInfo;

            if (IO::GetFileInfoSync(handle, handleInfo))
            {
                AddLog(
                    "GetFileInfoSync(handle): size=" +
                    std::to_string(handleInfo.size));
            }
            else
            {
                AddError("GetFileInfoSync(handle) failed.");
            }
            size_t poss = 0;
            IO::SeekFileSync(
                handle,
                poss,
                IO::SeekMode::START);

            std::array<char, 512> readBuffer{};
            bool eof = false;

            size_t read_bytes = readBuffer.size() - 1;
            size_t& read = read_bytes;
                IO::ReadFileSync(
                    handle,
                    readBuffer.data(),
                    read_bytes,
                    eof);
                
            readBuffer[std::min(
                read,
                readBuffer.size() - 1)] = '\0';

            AddLog(
                "ReadFileSync transferred=" +
                std::to_string(read) +
                " eof=" +
                std::to_string(static_cast<int>(eof)));

            AddLog(
                std::string("Read content: ") +
                readBuffer.data());

            if (!IO::CloseFileSync(handle))
                AddError("CloseFileSync failed.");
            else
                AddLog("CloseFileSync completed.");

            IO::FileInfo pathInfo;

            if (IO::GetFileInfoSync(file, pathInfo))
            {
                AddLog(
                    "GetFileInfoSync(path): size=" +
                    std::to_string(pathInfo.size));
            }
            else
            {
                AddError("GetFileInfoSync(path) failed.");
            }

            std::vector<IO::DirectoryEntry> entries;

            if (IO::ReadDirSync(dir, entries))
            {
                AddLog(
                    "ReadDirSync entries=" +
                    std::to_string(entries.size()));
            }
            else
            {
                AddError("ReadDirSync failed.");
            }

            if (!IO::RemoveFileSync(file))
                AddError("RemoveFileSync failed.");
            else
                AddLog("RemoveFileSync completed.");

            if (!IO::RemoveDirSync(dir))
                AddError("RemoveDirSync failed.");
            else
                AddLog("RemoveDirSync completed.");

            m_syncStatus = "Full synchronous suite finished.";

            AddLog("===== SYNC SUITE END =====");
            RefreshDirectory();
        }

        // =========================================================
        // ASYNCHRONOUS CHAIN
        // =========================================================

        inline void DrawAsyncChain()
        {
            ImGui::TextUnformatted(
                "One request at a time. Each callback queues the next step.");

            ImGui::Text(
                "Current step: %s",
                m_asyncStep.c_str());

            if (m_asyncRunning)
                ImGui::BeginDisabled();

            if (ImGui::Button("Run Full Async Chain"))
                StartAsyncChain();

            if (m_asyncRunning)
                ImGui::EndDisabled();
        }

        inline void StartAsyncChain()
        {
            if (m_asyncRunning)
                return;

            ++m_testRunId;

            m_asyncRunning = true;
            m_asyncStep = "CreateDirAsync";
            m_asyncDir =
                "./io_async_suite_" +
                std::to_string(m_testRunId);

            m_asyncFile =
                m_asyncDir + "/async_test.txt";

            m_asyncHandle.reset();

            m_asyncPayload =
                "MultiStation asynchronous IO chain\n"
                "create -> write -> tell -> seek -> read -> info -> close\n";

            std::fill(
                m_asyncReadBuffer.begin(),
                m_asyncReadBuffer.end(),
                '\0');

            AddLog("===== ASYNC CHAIN START =====");
            QueueAsyncCreateDir();
        }

        inline void FailAsyncChain(const std::string& message)
        {
            AddError(
                "Async chain failed at " +
                m_asyncStep +
                ": " +
                message);

            m_asyncRunning = false;
            m_asyncStep = "FAILED";

            if (m_asyncHandle)
            {
                const std::shared_ptr<IO::Handle> handle =
                    m_asyncHandle;

                m_asyncHandle.reset();

                IO::CloseFileAsync(
                    handle,
                    [](IO::Result&) {});
            }
        }

        inline void CompleteAsyncChain()
        {
            m_asyncRunning = false;
            m_asyncStep = "COMPLETED";

            AddLog("===== ASYNC CHAIN COMPLETED =====");
            RefreshDirectory();
        }

        inline void QueueAsyncCreateDir()
        {
            m_asyncStep = "CreateDirAsync";

            const bool accepted =
                IO::CreateDirAsync(
                    m_asyncDir,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "CreateDirAsync completion failed.");
                            return;
                        }

                        AddLog("CreateDirAsync completed.");
                        QueueAsyncCreateFile();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncCreateFile()
        {
            m_asyncStep = "CreateFileAsync";

            const bool accepted =
                IO::CreateFileAsync(
                    m_asyncFile,
                    IO::Access::READ | IO::Access::WRITE,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result) || !result.handle)
                        {
                            FailAsyncChain(
                                "CreateFileAsync completion failed.");
                            return;
                        }

                        m_asyncHandle = result.handle;

                        AddLog("CreateFileAsync completed.");
                        QueueAsyncWrite();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncWrite()
        {
            m_asyncStep = "WriteFileAsync";

            const bool accepted =
                IO::WriteFileAsync(
                    m_asyncHandle,
                    m_asyncPayload.data(),
                    m_asyncPayload.size(),
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "WriteFileAsync completion failed.");
                            return;
                        }

                        AddLog(
                            "WriteFileAsync transferred=" +
                            std::to_string(
                                result.transferred_bytes));

                        QueueAsyncTell();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncTell()
        {
            m_asyncStep = "TellFileAsync";

            const bool accepted =
                IO::TellFileAsync(
                    m_asyncHandle,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "TellFileAsync completion failed.");
                            return;
                        }

                        AddLog(
                            "TellFileAsync position=" +
                            std::to_string(result.pos));

                        QueueAsyncSeek();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncSeek()
        {
            m_asyncStep = "SeekFileAsync";

            const bool accepted =
                IO::SeekFileAsync(
                    m_asyncHandle,
                    0,
                    IO::SeekMode::START,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "SeekFileAsync completion failed.");
                            return;
                        }

                        AddLog(
                            "SeekFileAsync position=" +
                            std::to_string(result.pos));

                        QueueAsyncRead();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncRead()
        {
            m_asyncStep = "ReadFileAsync";

            const bool accepted =
                IO::ReadFileAsync(
                    m_asyncHandle,
                    m_asyncReadBuffer.data(),
                    m_asyncReadBuffer.size() - 1,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "ReadFileAsync completion failed.");
                            return;
                        }

                        const size_t read =
                            std::min(
                                result.transferred_bytes,
                                m_asyncReadBuffer.size() - 1);

                        m_asyncReadBuffer[read] = '\0';

                        AddLog(
                            "ReadFileAsync transferred=" +
                            std::to_string(read) +
                            " eof=" +
                            std::to_string(
                                static_cast<int>(result.eof)));

                        AddLog(
                            std::string("Read content: ") +
                            m_asyncReadBuffer.data());

                        QueueAsyncGetInfoHandle();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncGetInfoHandle()
        {
            m_asyncStep = "GetFileInfoAsync(handle)";

            const bool accepted =
                IO::GetFileInfoAsync(
                    m_asyncHandle,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "GetFileInfoAsync(handle) failed.");
                            return;
                        }

                        AddLog(
                            "GetFileInfoAsync(handle): size=" +
                            std::to_string(result.info.size));

                        QueueAsyncClose();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncClose()
        {
            m_asyncStep = "CloseFileAsync";

            const std::shared_ptr<IO::Handle> handle =
                m_asyncHandle;

            const bool accepted =
                IO::CloseFileAsync(
                    handle,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "CloseFileAsync completion failed.");
                            return;
                        }

                        m_asyncHandle.reset();

                        AddLog("CloseFileAsync completed.");
                        QueueAsyncGetInfoPath();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncGetInfoPath()
        {
            m_asyncStep = "GetFileInfoAsync(path)";

            const bool accepted =
                IO::GetFileInfoAsync(
                    m_asyncFile,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "GetFileInfoAsync(path) failed.");
                            return;
                        }

                        AddLog(
                            "GetFileInfoAsync(path): size=" +
                            std::to_string(result.info.size));

                        QueueAsyncReadDir();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncReadDir()
        {
            m_asyncStep = "ReadDirAsync";

            const bool accepted =
                IO::ReadDirAsync(
                    m_asyncDir,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "ReadDirAsync completion failed.");
                            return;
                        }

                        AddLog(
                            "ReadDirAsync entries=" +
                            std::to_string(result.entries.size()));

                        QueueAsyncRemoveFile();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncRemoveFile()
        {
            m_asyncStep = "RemoveFileAsync";

            const bool accepted =
                IO::RemoveFileAsync(
                    m_asyncFile,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "RemoveFileAsync completion failed.");
                            return;
                        }

                        AddLog("RemoveFileAsync completed.");
                        QueueAsyncRemoveDir();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        inline void QueueAsyncRemoveDir()
        {
            m_asyncStep = "RemoveDirAsync";

            const bool accepted =
                IO::RemoveDirAsync(
                    m_asyncDir,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            FailAsyncChain(
                                "RemoveDirAsync completion failed.");
                            return;
                        }

                        AddLog("RemoveDirAsync completed.");
                        CompleteAsyncChain();
                    });

            if (!accepted)
                FailAsyncChain("request rejected.");
        }

        // =========================================================
        // ASYNC BURST TEST
        // =========================================================

        struct BurstItem
        {
            size_t id = 0;
            std::string path;
            std::shared_ptr<IO::Handle> handle;
            std::array<char, 256> readBuffer{};
            bool finished = false;
        };

        inline void DrawBurstTest()
        {
            ImGui::TextUnformatted(
                "Queues many independent async file chains at once.");

            int count = static_cast<int>(m_burstTarget);

            if (ImGui::InputInt("Burst File Count", &count))
            {
                count = std::clamp(count, 1, 128);
                m_burstTarget = static_cast<size_t>(count);
            }

            ImGui::Text(
                "Completed: %zu / %zu | Failed: %zu",
                m_burstCompleted,
                m_burstTarget,
                m_burstFailed);

            if (m_burstRunning)
                ImGui::BeginDisabled();

            if (ImGui::Button("Run Async Burst Test"))
                StartBurstTest();

            if (m_burstRunning)
                ImGui::EndDisabled();
        }

        inline void StartBurstTest()
        {
            if (m_burstRunning)
                return;

            ++m_testRunId;

            m_burstRunning = true;
            m_burstCompleted = 0;
            m_burstFailed = 0;

            m_burstDir =
                "./io_burst_" +
                std::to_string(m_testRunId);

            m_burstItems.clear();
            m_burstItems.reserve(m_burstTarget);

            m_burstPayload =
                "MultiStation burst async payload\n";

            AddLog(
                "===== BURST TEST START: files=" +
                std::to_string(m_burstTarget) +
                " =====");

            const bool accepted =
                IO::CreateDirAsync(
                    m_burstDir,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            AddError(
                                "Burst CreateDirAsync failed.");
                            m_burstRunning = false;
                            return;
                        }

                        for (size_t i = 0; i < m_burstTarget; ++i)
                        {
                            auto item =
                                std::make_shared<BurstItem>();

                            item->id = i;
                            item->path =
                                m_burstDir +
                                "/file_" +
                                std::to_string(i) +
                                ".bin";

                            m_burstItems.push_back(item);
                            QueueBurstCreate(item);
                        }
                    });

            if (!accepted)
            {
                AddError("Burst CreateDirAsync rejected.");
                m_burstRunning = false;
            }
        }

        inline void QueueBurstCreate(
            const std::shared_ptr<BurstItem>& item)
        {
            const bool accepted =
                IO::CreateFileAsync(
                    item->path,
                    IO::Access::READ | IO::Access::WRITE,
                    [this, item](IO::Result& result)
                    {
                        if (!Completed(result) || !result.handle)
                        {
                            AbortBurstItem(
                                item,
                                "CreateFileAsync failed");
                            return;
                        }

                        item->handle = result.handle;
                        QueueBurstWrite(item);
                    });

            if (!accepted)
                AbortBurstItem(
                    item,
                    "CreateFileAsync rejected");
        }

        inline void QueueBurstWrite(
            const std::shared_ptr<BurstItem>& item)
        {
            const bool accepted =
                IO::WriteFileAsync(
                    item->handle,
                    m_burstPayload.data(),
                    m_burstPayload.size(),
                    [this, item](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            AbortBurstItem(
                                item,
                                "WriteFileAsync failed");
                            return;
                        }

                        QueueBurstSeek(item);
                    });

            if (!accepted)
                AbortBurstItem(
                    item,
                    "WriteFileAsync rejected");
        }

        inline void QueueBurstSeek(
            const std::shared_ptr<BurstItem>& item)
        {
            const bool accepted =
                IO::SeekFileAsync(
                    item->handle,
                    0,
                    IO::SeekMode::START,
                    [this, item](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            AbortBurstItem(
                                item,
                                "SeekFileAsync failed");
                            return;
                        }

                        QueueBurstRead(item);
                    });

            if (!accepted)
                AbortBurstItem(
                    item,
                    "SeekFileAsync rejected");
        }

        inline void QueueBurstRead(
            const std::shared_ptr<BurstItem>& item)
        {
            const bool accepted =
                IO::ReadFileAsync(
                    item->handle,
                    item->readBuffer.data(),
                    item->readBuffer.size(),
                    [this, item](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            AbortBurstItem(
                                item,
                                "ReadFileAsync failed");
                            return;
                        }

                        const size_t expected =
                            m_burstPayload.size();

                        if (result.transferred_bytes != expected)
                        {
                            AbortBurstItem(
                                item,
                                "Read byte count mismatch");
                            return;
                        }

                        if (!std::equal(
                            m_burstPayload.begin(),
                            m_burstPayload.end(),
                            item->readBuffer.begin()))
                        {
                            AbortBurstItem(
                                item,
                                "Read data mismatch");
                            return;
                        }

                        QueueBurstInfo(item);
                    });

            if (!accepted)
                AbortBurstItem(
                    item,
                    "ReadFileAsync rejected");
        }

        inline void QueueBurstInfo(
            const std::shared_ptr<BurstItem>& item)
        {
            const bool accepted =
                IO::GetFileInfoAsync(
                    item->handle,
                    [this, item](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            AbortBurstItem(
                                item,
                                "GetFileInfoAsync failed");
                            return;
                        }

                        QueueBurstClose(item, true);
                    });

            if (!accepted)
                AbortBurstItem(
                    item,
                    "GetFileInfoAsync rejected");
        }

        inline void QueueBurstClose(
            const std::shared_ptr<BurstItem>& item,
            bool successSoFar)
        {
            if (!item->handle)
            {
                QueueBurstRemove(item, successSoFar);
                return;
            }

            const std::shared_ptr<IO::Handle> handle =
                item->handle;

            const bool accepted =
                IO::CloseFileAsync(
                    handle,
                    [this, item, successSoFar](IO::Result& result)
                    {
                        item->handle.reset();

                        const bool success =
                            successSoFar &&
                            Completed(result);

                        QueueBurstRemove(item, success);
                    });

            if (!accepted)
            {
                item->handle.reset();
                QueueBurstRemove(item, false);
            }
        }

        inline void QueueBurstRemove(
            const std::shared_ptr<BurstItem>& item,
            bool successSoFar)
        {
            const bool accepted =
                IO::RemoveFileAsync(
                    item->path,
                    [this, item, successSoFar](IO::Result& result)
                    {
                        const bool success =
                            successSoFar &&
                            Completed(result);

                        CompleteBurstItem(
                            item,
                            success);
                    });

            if (!accepted)
                CompleteBurstItem(item, false);
        }

        inline void AbortBurstItem(
            const std::shared_ptr<BurstItem>& item,
            const std::string& reason)
        {
            if (item->finished)
                return;

            AddError(
                "Burst item " +
                std::to_string(item->id) +
                ": " +
                reason);

            QueueBurstClose(item, false);
        }

        inline void CompleteBurstItem(
            const std::shared_ptr<BurstItem>& item,
            bool success)
        {
            if (item->finished)
                return;

            item->finished = true;

            ++m_burstCompleted;

            if (!success)
                ++m_burstFailed;

            if (m_burstCompleted == m_burstTarget)
                FinishBurstTest();
        }

        inline void FinishBurstTest()
        {
            AddLog(
                "Burst file chains completed. failures=" +
                std::to_string(m_burstFailed));

            const bool accepted =
                IO::ReadDirAsync(
                    m_burstDir,
                    [this](IO::Result& result)
                    {
                        if (Completed(result))
                        {
                            AddLog(
                                "Burst directory entries after cleanup=" +
                                std::to_string(
                                    result.entries.size()));
                        }
                        else
                        {
                            AddError(
                                "Final burst ReadDirAsync failed.");
                        }

                        const bool removeAccepted =
                            IO::RemoveDirAsync(
                                m_burstDir,
                                [this](IO::Result& removeResult)
                                {
                                    if (Completed(removeResult))
                                        AddLog(
                                            "Burst directory removed.");
                                    else
                                        AddError(
                                            "Burst directory removal failed.");

                                    m_burstRunning = false;
                                    AddLog(
                                        "===== BURST TEST END =====");

                                    RefreshDirectory();
                                });

                        if (!removeAccepted)
                        {
                            AddError(
                                "Burst RemoveDirAsync rejected.");
                            m_burstRunning = false;
                        }
                    });

            if (!accepted)
            {
                AddError(
                    "Final burst ReadDirAsync rejected.");
                m_burstRunning = false;
            }
        }

        // =========================================================
        // HEAVY ASYNC WRITER
        // =========================================================

        inline void DrawHeavyWriter()
        {
            ImGui::InputText(
                "Heavy File Name",
                m_heavyFileName,
                sizeof(m_heavyFileName));

            ImGui::Text(
                "Target: %.2f GiB",
                static_cast<double>(m_heavyTargetSize) /
                (1024.0 * 1024.0 * 1024.0));

            ImGui::Text(
                "Chunk: %zu KiB",
                m_heavyWriteBuffer.size() / 1024);

            if (m_heavyWriting)
            {
                const float progress =
                    m_heavyTargetSize == 0
                    ? 0.0f
                    : static_cast<float>(
                        static_cast<double>(m_heavyBytesWritten) /
                        static_cast<double>(m_heavyTargetSize));

                ImGui::ProgressBar(
                    progress,
                    ImVec2(-1.0f, 0.0f));

                ImGui::Text(
                    "Written: %.2f MiB / %.2f MiB",
                    static_cast<double>(m_heavyBytesWritten) /
                    (1024.0 * 1024.0),
                    static_cast<double>(m_heavyTargetSize) /
                    (1024.0 * 1024.0));
            }
            else
            {
                if (ImGui::Button("Create 1 GiB Async Test File"))
                    StartHeavyWrite();
            }

            if (!m_heavyStatus.empty())
                ImGui::TextWrapped("%s", m_heavyStatus.c_str());
        }

        inline void StartHeavyWrite()
        {
            if (m_heavyWriting)
                return;

            const std::filesystem::path path =
                std::filesystem::path(m_currentPath) /
                m_heavyFileName;

            m_heavyFilePath = path.string();
            m_heavyBytesWritten = 0;
            m_heavyHandle.reset();
            m_heavyWriting = true;
            m_heavyStatus = "CreateFileAsync pending...";

            const bool accepted =
                IO::CreateFileAsync(
                    m_heavyFilePath,
                    IO::Access::WRITE,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result) || !result.handle)
                        {
                            m_heavyWriting = false;
                            m_heavyStatus =
                                "CreateFileAsync failed.";
                            AddError(m_heavyStatus);
                            return;
                        }

                        m_heavyHandle = result.handle;
                        m_heavyStatus = "Writing...";
                        AddLog(
                            "Heavy file created. Starting chained writes.");

                        QueueNextHeavyWrite();
                    });

            if (!accepted)
            {
                m_heavyWriting = false;
                m_heavyStatus =
                    "CreateFileAsync rejected.";
                AddError(m_heavyStatus);
            }
        }

        inline void QueueNextHeavyWrite()
        {
            if (!m_heavyWriting || !m_heavyHandle)
                return;

            if (m_heavyBytesWritten >= m_heavyTargetSize)
            {
                FinishHeavyWrite();
                return;
            }

            const size_t remaining =
                m_heavyTargetSize -
                m_heavyBytesWritten;

            const size_t bytesToWrite =
                std::min(
                    remaining,
                    m_heavyWriteBuffer.size());

            const bool accepted =
                IO::WriteFileAsync(
                    m_heavyHandle,
                    m_heavyWriteBuffer.data(),
                    bytesToWrite,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result))
                        {
                            m_heavyStatus =
                                "WriteFileAsync failed.";
                            AddError(m_heavyStatus);
                            CloseHeavyAfterFailure();
                            return;
                        }

                        if (result.transferred_bytes == 0)
                        {
                            m_heavyStatus =
                                "WriteFileAsync completed with 0 bytes.";
                            AddError(m_heavyStatus);
                            CloseHeavyAfterFailure();
                            return;
                        }

                        m_heavyBytesWritten +=
                            result.transferred_bytes;

                        QueueNextHeavyWrite();
                    });

            if (!accepted)
            {
                m_heavyStatus =
                    "WriteFileAsync rejected.";
                AddError(m_heavyStatus);
                CloseHeavyAfterFailure();
            }
        }

        inline void FinishHeavyWrite()
        {
            if (!m_heavyHandle)
            {
                m_heavyWriting = false;
                return;
            }

            const std::shared_ptr<IO::Handle> handle =
                m_heavyHandle;

            const bool accepted =
                IO::CloseFileAsync(
                    handle,
                    [this](IO::Result& result)
                    {
                        if (Completed(result))
                        {
                            m_heavyStatus =
                                "1 GiB async file completed.";
                            AddLog(m_heavyStatus);
                            RefreshDirectory();
                        }
                        else
                        {
                            m_heavyStatus =
                                "Heavy file close failed.";
                            AddError(m_heavyStatus);
                        }

                        m_heavyHandle.reset();
                        m_heavyWriting = false;
                    });

            if (!accepted)
            {
                m_heavyStatus =
                    "CloseFileAsync rejected.";
                AddError(m_heavyStatus);
                m_heavyWriting = false;
            }
        }

        inline void CloseHeavyAfterFailure()
        {
            if (!m_heavyHandle)
            {
                m_heavyWriting = false;
                return;
            }

            const std::shared_ptr<IO::Handle> handle =
                m_heavyHandle;

            const bool accepted =
                IO::CloseFileAsync(
                    handle,
                    [this](IO::Result&)
                    {
                        m_heavyHandle.reset();
                        m_heavyWriting = false;
                    });

            if (!accepted)
            {
                m_heavyWriting = false;
                AddError(
                    "CloseFileAsync rejected after heavy-write failure.");
            }
        }

        // =========================================================
        // SLOW ASYNC READER
        // =========================================================

        inline void DrawSlowReader()
        {
            ImGui::Text(
                "File: %s",
                m_slowReadPath.empty()
                ? "<none>"
                : m_slowReadPath.c_str());

            int chunkSize =
                static_cast<int>(m_slowReadChunkSize);

            if (ImGui::InputInt(
                "Bytes Per Slow Read",
                &chunkSize))
            {
                chunkSize = std::clamp(
                    chunkSize,
                    1,
                    4096);

                if (!m_slowReadPending)
                {
                    m_slowReadChunkSize =
                        static_cast<size_t>(chunkSize);

                    m_slowReadBuffer.resize(
                        m_slowReadChunkSize + 1);
                }
            }

            ImGui::SliderFloat(
                "Seconds Between Reads",
                &m_slowReadInterval,
                0.0f,
                5.0f);

            if (m_slowReading)
            {
                ImGui::Text(
                    "Bytes read: %zu",
                    m_slowTotalRead);

                ImGui::Text(
                    "State: %s",
                    m_slowReadPending
                    ? "Read pending"
                    : "Waiting");

                if (ImGui::Button("Stop Slow Reader"))
                    StopSlowRead();
            }

            ImGui::BeginChild(
                "##SlowReadPreview",
                ImVec2(0.0f, 180.0f),
                true,
                ImGuiWindowFlags_HorizontalScrollbar);

            ImGui::TextUnformatted(
                m_slowReadPreview.c_str());

            ImGui::EndChild();
        }

        inline void StartSlowRead(const std::string& path)
        {
            if (m_slowReading)
                return;

            m_slowReadPath = path;
            m_slowReadPreview.clear();
            m_slowTotalRead = 0;
            m_slowEOF = false;
            m_slowReadPending = false;
            m_slowStopRequested = false;
            m_slowHandle.reset();
            m_slowReading = true;
            m_nextSlowReadTime = ImGui::GetTime();

            const bool accepted =
                IO::OpenFileAsync(
                    path,
                    IO::Access::READ,
                    [this](IO::Result& result)
                    {
                        if (!Completed(result) || !result.handle)
                        {
                            m_slowReading = false;
                            AddError(
                                "Slow reader OpenFileAsync failed.");
                            return;
                        }

                        m_slowHandle = result.handle;
                        m_nextSlowReadTime =
                            ImGui::GetTime();

                        AddLog(
                            "Slow reader opened: " +
                            m_slowReadPath);
                    });

            if (!accepted)
            {
                m_slowReading = false;
                AddError(
                    "Slow reader OpenFileAsync rejected.");
            }
        }

        inline void UpdateSlowReader()
        {
            if (!m_slowReading ||
                !m_slowHandle ||
                m_slowReadPending ||
                m_slowEOF ||
                m_slowStopRequested)
            {
                return;
            }

            if (ImGui::GetTime() < m_nextSlowReadTime)
                return;

            QueueNextSlowRead();
        }

        inline void QueueNextSlowRead()
        {
            if (!m_slowHandle ||
                m_slowReadPending ||
                m_slowEOF ||
                m_slowStopRequested)
            {
                return;
            }

            m_slowReadPending = true;

            const bool accepted =
                IO::ReadFileAsync(
                    m_slowHandle,
                    m_slowReadBuffer.data(),
                    m_slowReadChunkSize,
                    [this](IO::Result& result)
                    {
                        m_slowReadPending = false;

                        if (!Completed(result))
                        {
                            AddError(
                                "Slow reader ReadFileAsync failed.");
                            CloseSlowRead();
                            return;
                        }

                        const size_t read =
                            std::min(
                                result.transferred_bytes,
                                m_slowReadChunkSize);

                        m_slowTotalRead += read;

                        if (read > 0)
                        {
                            m_slowReadPreview.append(
                                m_slowReadBuffer.data(),
                                read);

                            constexpr size_t MAX_PREVIEW =
                                64ull * 1024ull;

                            if (m_slowReadPreview.size() >
                                MAX_PREVIEW)
                            {
                                m_slowReadPreview.erase(
                                    0,
                                    m_slowReadPreview.size() -
                                    MAX_PREVIEW);
                            }
                        }

                        m_slowEOF = result.eof;

                        // Important: Stop may have been pressed while
                        // the read request was still in flight.
                        if (m_slowStopRequested || m_slowEOF)
                        {
                            CloseSlowRead();
                            return;
                        }

                        m_nextSlowReadTime =
                            ImGui::GetTime() +
                            static_cast<double>(
                                m_slowReadInterval);
                    });

            if (!accepted)
            {
                m_slowReadPending = false;
                AddError(
                    "Slow reader ReadFileAsync rejected.");
                CloseSlowRead();
            }
        }

        inline void StopSlowRead()
        {
            if (!m_slowReading)
                return;

            m_slowStopRequested = true;

            if (!m_slowReadPending)
                CloseSlowRead();
        }

        inline void CloseSlowRead()
        {
            if (!m_slowHandle)
            {
                m_slowReading = false;
                m_slowReadPending = false;
                m_slowStopRequested = false;
                return;
            }

            const std::shared_ptr<IO::Handle> handle =
                m_slowHandle;

            const bool accepted =
                IO::CloseFileAsync(
                    handle,
                    [this](IO::Result& result)
                    {
                        if (Completed(result))
                        {
                            AddLog(
                                "Slow reader closed after " +
                                std::to_string(m_slowTotalRead) +
                                " bytes.");
                        }
                        else
                        {
                            AddError(
                                "Slow reader CloseFileAsync failed.");
                        }

                        m_slowHandle.reset();
                        m_slowReading = false;
                        m_slowReadPending = false;
                        m_slowStopRequested = false;
                    });

            if (!accepted)
            {
                AddError(
                    "Slow reader CloseFileAsync rejected.");
                m_slowReading = false;
                m_slowReadPending = false;
                m_slowStopRequested = false;
            }
        }

    private:
        bool m_initialized = false;
        size_t m_testRunId = 0;

        // Directory browser
        bool m_directoryLoading = false;
        std::string m_currentPath = ".";
        std::vector<IO::DirectoryEntry> m_entries;
        int m_selectedIndex = -1;
        char m_pathBuffer[1024]{};

        // Log
        std::vector<std::string> m_log;

        // Sync suite
        std::string m_syncStatus;

        // Async chain
        bool m_asyncRunning = false;
        std::string m_asyncStep = "IDLE";
        std::string m_asyncDir;
        std::string m_asyncFile;
        std::string m_asyncPayload;
        std::shared_ptr<IO::Handle> m_asyncHandle;
        std::vector<char> m_asyncReadBuffer;

        // Burst test
        bool m_burstRunning = false;
        size_t m_burstTarget = 24;
        size_t m_burstCompleted = 0;
        size_t m_burstFailed = 0;
        std::string m_burstDir;
        std::string m_burstPayload;
        std::vector<std::shared_ptr<BurstItem>> m_burstItems;

        // Heavy writer
        size_t m_heavyTargetSize =
            1024ull * 1024ull * 1024ull;

        size_t m_heavyBytesWritten = 0;
        bool m_heavyWriting = false;
        std::shared_ptr<IO::Handle> m_heavyHandle;
        std::vector<char> m_heavyWriteBuffer;
        std::string m_heavyFilePath;
        std::string m_heavyStatus;
        char m_heavyFileName[256] = "heavy_test.bin";

        // Slow reader
        bool m_slowReading = false;
        bool m_slowReadPending = false;
        bool m_slowEOF = false;
        bool m_slowStopRequested = false;
        std::shared_ptr<IO::Handle> m_slowHandle;
        std::string m_slowReadPath;
        std::vector<char> m_slowReadBuffer;
        std::string m_slowReadPreview;
        size_t m_slowReadChunkSize = 32;
        size_t m_slowTotalRead = 0;
        float m_slowReadInterval = 1.0f;
        double m_nextSlowReadTime = 0.0;
    };


    // The example module is intentionally IO-only.
    // The system itself does not test keyboard/events or unrelated features.
    class ExampleSystem : public ISystem
    {
    public:
        virtual inline void OnAttach(void) noexcept override
        {
            MS_INFO("IO Example System On Attach");
        }

        virtual inline void OnDettach(void) noexcept override
        {
            MS_INFO("IO Example System On Detach");
        }

        virtual inline void OnUpdate(float) noexcept override
        {
        }

        virtual inline void OnEvent(Event&) noexcept override
        {
        }
    };


    class ExampleModule : public ISystemModule
    {
    public:
        virtual inline void Init(EngineContext&) noexcept override
        {
            MS_INFO("IO Example Module Initialized");
            Editor::AddDrawable(&m_panel);
        }

        virtual inline void Fini(EngineContext&) noexcept override
        {
            MS_INFO("IO Example Module Finalized");
        }

        virtual inline ISystem& GetSystem(void) noexcept override
        {
            return m_system;
        }

    private:
        ExampleSystem m_system;
        ExamplePanel m_panel;
    };
}
