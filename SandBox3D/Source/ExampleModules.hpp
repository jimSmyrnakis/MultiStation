#pragma once
#include "mspch.h"
#include <Application.hpp>
#include <IO.hpp>
#include <filesystem>
#include <algorithm>
#include <cstdio>
namespace MultiStation{

    class ExamplePanel : public IDrawable
    {
    public:

        virtual inline void Draw(void) override
        {
            if (!m_initialized)
            {
                m_initialized = true;

                /*
                    1 MB write buffer.

                    We keep this buffer alive as a member because
                    WriteFileAsync() requires the source buffer
                    to remain valid until the callback executes.
                */
                m_writeBuffer.resize(
                    1024ull * 1024ull
                );

                /*
                    Fill the heavy-file chunk with some visible
                    repeating data rather than zeroes.
                */
                static const char pattern[] =
                    "MultiStation Async IO Stress Test\n";

                for (
                    size_t i = 0;
                    i < m_writeBuffer.size();
                    ++i
                    )
                {
                    m_writeBuffer[i] =
                        pattern[
                            i %
                                (sizeof(pattern) - 1)
                        ];
                }

                m_readBuffer.resize(
                    m_readChunkSize + 1
                );

                SetDirectory(".");
            }


            /*
                Slow-read scheduling.

                Only submit a new async read after:
                    - previous read completed
                    - requested delay elapsed
            */
            UpdateSlowReader();


            ImGui::Begin("IO File Browser");


            // =================================================
            // DIRECTORY BROWSER
            // =================================================

            DrawNavigationBar();

            ImGui::Separator();

            if (m_loading)
            {
                ImGui::TextDisabled(
                    "Loading directory..."
                );

                ImGui::Separator();
            }

            if (!m_error.empty())
            {
                ImGui::TextWrapped(
                    "Error: %s",
                    m_error.c_str()
                );

                ImGui::Separator();
            }


            ImGui::Text(
                "Current Directory: %s",
                m_currentPath.c_str()
            );

            ImGui::Text(
                "Entries: %zu",
                m_entries.size()
            );

            ImGui::Separator();

            DrawDirectoryContents();

            ImGui::Separator();

            DrawSelectedInfo();


            // =================================================
            // HEAVY FILE CREATOR
            // =================================================

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "Heavy File Generator",
                ImGuiTreeNodeFlags_DefaultOpen
            ))
            {
                DrawHeavyFileGenerator();
            }


            // =================================================
            // ASYNC FILE READER
            // =================================================

            ImGui::Separator();

            if (ImGui::CollapsingHeader(
                "Async Slow File Reader",
                ImGuiTreeNodeFlags_DefaultOpen
            ))
            {
                DrawSlowReader();
            }


            ImGui::End();
        }


    private:

        // =====================================================
        // DIRECTORY UI
        // =====================================================

        inline void DrawNavigationBar()
        {
            if (m_loading)
                ImGui::BeginDisabled();

            if (ImGui::Button("Up"))
            {
                GoUp();
            }

            ImGui::SameLine();

            if (ImGui::Button("Refresh"))
            {
                RefreshDirectory();
            }

            ImGui::SameLine();

            ImGui::SetNextItemWidth(-1.0f);

            const bool enter =
                ImGui::InputText(
                    "##Path",
                    m_pathBuffer,
                    sizeof(m_pathBuffer),
                    ImGuiInputTextFlags_EnterReturnsTrue
                );

            if (enter)
            {
                SetDirectory(
                    m_pathBuffer
                );
            }

            if (m_loading)
                ImGui::EndDisabled();
        }


        inline void DrawDirectoryContents()
        {
            if (ImGui::BeginChild(
                "##DirectoryContents",
                ImVec2(0.0f, 300.0f),
                true
            ))
            {
                for (
                    size_t i = 0;
                    i < m_entries.size();
                    ++i
                    )
                {
                    const IO::DirectoryEntry& entry =
                        m_entries[i];

                    ImGui::PushID(
                        static_cast<int>(i)
                    );


                    const char* prefix =
                        entry.is_directory
                        ? "[DIR] "
                        : "[FILE] ";


                    std::string label =
                        prefix +
                        entry.name;


                    const bool selected =
                        m_selectedIndex ==
                        static_cast<int>(i);


                    if (ImGui::Selectable(
                        label.c_str(),
                        selected,
                        ImGuiSelectableFlags_AllowDoubleClick
                    ))
                    {
                        m_selectedIndex =
                            static_cast<int>(i);


                        if (
                            ImGui::IsMouseDoubleClicked(
                                ImGuiMouseButton_Left
                            )
                            )
                        {
                            /*
                                Copy before any async operation can
                                eventually replace m_entries.
                            */
                            const std::string path =
                                entry.path;

                            const bool isDirectory =
                                entry.is_directory;

                            ImGui::PopID();


                            if (isDirectory)
                            {
                                if (!m_loading)
                                    SetDirectory(path);
                            }
                            else
                            {
                                /*
                                    Double-clicking any file starts
                                    the async slow reader.
                                */
                                StartSlowRead(path);
                            }

                            break;
                        }
                    }


                    ImGui::PopID();
                }
            }

            ImGui::EndChild();
        }


        inline void DrawSelectedInfo()
        {
            if (
                m_selectedIndex < 0 ||
                m_selectedIndex >=
                static_cast<int>(
                    m_entries.size()
                    )
                )
            {
                ImGui::TextDisabled(
                    "No entry selected."
                );

                return;
            }


            const IO::DirectoryEntry& entry =
                m_entries[
                    m_selectedIndex
                ];


            ImGui::Text(
                "Name: %s",
                entry.name.c_str()
            );

            ImGui::TextWrapped(
                "Path: %s",
                entry.path.c_str()
            );

            ImGui::Text(
                "Type: %s",
                entry.is_directory
                ? "Directory"
                : "File"
            );


            if (!entry.is_directory)
            {
                ImGui::Text(
                    "Size: %zu bytes",
                    entry.size
                );
            }


            if (entry.is_directory)
            {
                if (m_loading)
                    ImGui::BeginDisabled();

                if (ImGui::Button(
                    "Open Directory"
                ))
                {
                    const std::string path =
                        entry.path;

                    SetDirectory(path);
                }

                if (m_loading)
                    ImGui::EndDisabled();
            }
            else
            {
                if (m_reading)
                    ImGui::BeginDisabled();

                if (ImGui::Button(
                    "Open Async Reader"
                ))
                {
                    const std::string path =
                        entry.path;

                    StartSlowRead(path);
                }

                if (m_reading)
                    ImGui::EndDisabled();
            }
        }


        // =====================================================
        // DIRECTORY ASYNC
        // =====================================================

        inline bool SetDirectory(
            const std::string& path)
        {
            if (m_loading)
                return false;


            m_loading = true;
            m_error.clear();


            const std::string requestedPath =
                path;


            const bool accepted =
                IO::ReadDirAsync(
                    requestedPath,

                    [this, requestedPath]
            (IO::Result& result)
                    {
                        m_loading = false;


                        if (
                            result.status !=
                            IO::Status::COMPLETED
                            )
                        {
                            m_error =
                                "Failed to read directory: " +
                                requestedPath;

                            MS_ERROR(
                                "ReadDirAsync failed: %s | Error: %d",
                                requestedPath.c_str(),
                                static_cast<int>(
                                    result.error
                                    )
                            );

                            return;
                        }


                        std::vector<
                            IO::DirectoryEntry
                        > entries =
                            std::move(
                                result.entries
                            );


                        std::sort(
                            entries.begin(),
                            entries.end(),

                            [](
                                const IO::DirectoryEntry& a,
                                const IO::DirectoryEntry& b
                                )
                            {
                                if (
                                    a.is_directory !=
                                    b.is_directory
                                    )
                                {
                                    return
                                        a.is_directory >
                                        b.is_directory;
                                }

                                return
                                    a.name <
                                    b.name;
                            }
                        );


                        m_currentPath =
                            requestedPath;

                        m_entries =
                            std::move(entries);

                        m_selectedIndex = -1;

                        m_error.clear();


                        std::snprintf(
                            m_pathBuffer,
                            sizeof(m_pathBuffer),
                            "%s",
                            m_currentPath.c_str()
                        );


                        MS_INFO(
                            "Opened directory async: %s | Entries: %zu",
                            m_currentPath.c_str(),
                            m_entries.size()
                        );
                    }
                );


            if (!accepted)
            {
                m_loading = false;

                m_error =
                    "Failed to queue directory request: " +
                    requestedPath;

                return false;
            }


            return true;
        }


        inline void RefreshDirectory()
        {
            if (m_loading)
                return;

            SetDirectory(
                m_currentPath
            );
        }


        inline void GoUp()
        {
            if (m_loading)
                return;


            std::filesystem::path path(
                m_currentPath
            );


            if (
                path.empty() ||
                path == "."
                )
            {
                path =
                    std::filesystem::absolute(
                        path
                    );
            }


            const std::filesystem::path parent =
                path.parent_path();


            if (parent.empty())
                return;


            SetDirectory(
                parent.string()
            );
        }


        // =====================================================
        // HEAVY FILE GENERATOR UI
        // =====================================================

        inline void DrawHeavyFileGenerator()
        {
            ImGui::InputText(
                "File Name",
                m_heavyFileName,
                sizeof(m_heavyFileName)
            );


            ImGui::Text(
                "Target size: %.2f GB",
                static_cast<double>(
                    m_heavyTargetSize
                    ) /
                (
                    1024.0 *
                    1024.0 *
                    1024.0
                    )
            );


            ImGui::Text(
                "Write chunk: %zu KB",
                m_writeBuffer.size() /
                1024
            );


            if (m_creatingHeavyFile)
            {
                const float progress =
                    m_heavyTargetSize > 0
                    ?
                    static_cast<float>(
                        static_cast<double>(
                            m_heavyBytesWritten
                            ) /
                        static_cast<double>(
                            m_heavyTargetSize
                            )
                        )
                    :
                    0.0f;


                ImGui::ProgressBar(
                    progress,
                    ImVec2(-1.0f, 0.0f)
                );


                ImGui::Text(
                    "Written: %.2f MB / %.2f MB",
                    static_cast<double>(
                        m_heavyBytesWritten
                        ) /
                    (1024.0 * 1024.0),

                    static_cast<double>(
                        m_heavyTargetSize
                        ) /
                    (1024.0 * 1024.0)
                );


                ImGui::TextDisabled(
                    "Writing asynchronously..."
                );
            }
            else
            {
                if (ImGui::Button(
                    "Create 1 GB Test File"
                ))
                {
                    StartHeavyFileCreation();
                }
            }


            if (!m_heavyStatus.empty())
            {
                ImGui::TextWrapped(
                    "%s",
                    m_heavyStatus.c_str()
                );
            }
        }


        // =====================================================
        // HEAVY FILE CREATION
        // =====================================================

        inline void StartHeavyFileCreation()
        {
            if (m_creatingHeavyFile)
                return;


            std::filesystem::path path =
                std::filesystem::path(
                    m_currentPath
                ) /
                m_heavyFileName;


            m_heavyFilePath =
                path.string();


            m_heavyBytesWritten = 0;
            m_heavyHandle = nullptr;

            m_creatingHeavyFile = true;

            m_heavyStatus =
                "Opening output file...";


            const bool accepted =
                IO::CreateFileAsync(
                    m_heavyFilePath,

                    IO::Access::WRITE,

                    [this](IO::Result& result)
                    {
                        if (
                            result.status !=
                            IO::Status::COMPLETED ||
                            !result.handle
                            )
                        {
                            m_creatingHeavyFile =
                                false;

                            m_heavyStatus =
                                "Failed to create heavy file.";

                            return;
                        }


                        m_heavyHandle =
                            result.handle;


                        m_heavyStatus =
                            "Writing heavy file...";


                        /*
                            Start first asynchronous chunk.
                        */
                        QueueNextHeavyWrite();
                    }
                );


            if (!accepted)
            {
                m_creatingHeavyFile = false;

                m_heavyStatus =
                    "CreateFileAsync request rejected.";
            }
        }


        inline void QueueNextHeavyWrite()
        {
            if (
                !m_creatingHeavyFile ||
                !m_heavyHandle
                )
                return;


            if (
                m_heavyBytesWritten >=
                m_heavyTargetSize
                )
            {
                FinishHeavyFileCreation();
                return;
            }


            const size_t remaining =
                m_heavyTargetSize -
                m_heavyBytesWritten;


            const size_t bytesToWrite =
                std::min(
                    remaining,
                    m_writeBuffer.size()
                );


            const bool accepted =
                IO::WriteFileAsync(
                    m_heavyHandle,
                    m_writeBuffer.data(),
                    bytesToWrite,

                    [this, bytesToWrite]
            (IO::Result& result)
                    {
                        if (
                            result.status !=
                            IO::Status::COMPLETED
                            )
                        {
                            m_heavyStatus =
                                "WriteFileAsync failed.";

                            CloseHeavyFileAfterFailure();

                            return;
                        }


                        m_heavyBytesWritten +=
                            result.transferred_bytes;


                        /*
                            Sequential chain:

                            only issue the next write after
                            the previous one completed.
                        */
                        QueueNextHeavyWrite();
                    }
                );


            if (!accepted)
            {
                m_heavyStatus =
                    "Write request rejected.";

                CloseHeavyFileAfterFailure();
            }
        }


        inline void FinishHeavyFileCreation()
        {
            if (!m_heavyHandle)
            {
                m_creatingHeavyFile = false;
                return;
            }


            IO::Handle* handle =
                m_heavyHandle;


            m_heavyHandle = nullptr;


            const bool accepted =
                IO::CloseFileAsync(
                    handle,

                    [this](IO::Result& result)
                    {
                        m_creatingHeavyFile =
                            false;


                        if (
                            result.status ==
                            IO::Status::COMPLETED
                            )
                        {
                            m_heavyStatus =
                                "1 GB test file completed.";

                            /*
                                Refresh browser so the new
                                file becomes visible.
                            */
                            RefreshDirectory();
                        }
                        else
                        {
                            m_heavyStatus =
                                "File written, but close failed.";
                        }
                    }
                );


            if (!accepted)
            {
                m_creatingHeavyFile = false;

                m_heavyStatus =
                    "Close request rejected.";
            }
        }


        inline void CloseHeavyFileAfterFailure()
        {
            if (!m_heavyHandle)
            {
                m_creatingHeavyFile = false;
                return;
            }


            IO::Handle* handle =
                m_heavyHandle;

            m_heavyHandle = nullptr;


            IO::CloseFileAsync(
                handle,

                [this](IO::Result&)
                {
                    m_creatingHeavyFile =
                        false;
                }
            );
        }


        // =====================================================
        // SLOW READER UI
        // =====================================================

        inline void DrawSlowReader()
        {
            ImGui::Text(
                "File: %s",
                m_readFilePath.empty()
                ? "<none>"
                : m_readFilePath.c_str()
            );


            int chunkSize =
                static_cast<int>(
                    m_readChunkSize
                    );


            if (ImGui::InputInt(
                "Bytes Per Read",
                &chunkSize
            ))
            {
                if (chunkSize < 1)
                    chunkSize = 1;

                if (chunkSize > 4096)
                    chunkSize = 4096;


                /*
                    Do not resize a buffer while an async
                    request is using it.
                */
                if (!m_readRequestPending)
                {
                    m_readChunkSize =
                        static_cast<size_t>(
                            chunkSize
                            );

                    m_readBuffer.resize(
                        m_readChunkSize + 1
                    );
                }
            }


            ImGui::SliderFloat(
                "Seconds Between Reads",
                &m_readIntervalSeconds,
                0.0f,
                5.0f
            );


            if (m_reading)
            {
                ImGui::Text(
                    "Bytes read: %zu",
                    m_totalReadBytes
                );


                if (m_readRequestPending)
                {
                    ImGui::TextDisabled(
                        "Read request pending..."
                    );
                }
                else
                {
                    ImGui::TextDisabled(
                        "Waiting for next chunk..."
                    );
                }


                if (ImGui::Button(
                    "Stop Reading"
                ))
                {
                    StopSlowRead();
                }
            }


            ImGui::Separator();


            ImGui::Text(
                "Read Preview"
            );


            ImGui::BeginChild(
                "##AsyncReadPreview",
                ImVec2(0.0f, 180.0f),
                true,
                ImGuiWindowFlags_HorizontalScrollbar
            );


            ImGui::TextUnformatted(
                m_readPreview.c_str()
            );


            ImGui::EndChild();
        }


        // =====================================================
        // SLOW ASYNC READER
        // =====================================================

        inline void StartSlowRead(
            const std::string& path)
        {
            if (m_reading)
                return;


            m_readFilePath =
                path;

            m_readPreview.clear();

            m_totalReadBytes = 0;

            m_readEOF = false;

            m_readRequestPending = false;

            m_readHandle = nullptr;

            m_reading = true;

            m_nextReadTime =
                ImGui::GetTime();


            const bool accepted =
                IO::OpenFileAsync(
                    path,
                    IO::Access::READ,

                    [this](IO::Result& result)
                    {
                        if (
                            result.status !=
                            IO::Status::COMPLETED ||
                            !result.handle
                            )
                        {
                            m_reading = false;

                            m_error =
                                "Failed to open file for async reading.";

                            return;
                        }


                        m_readHandle =
                            result.handle;


                        /*
                            Allow first read immediately.
                        */
                        m_nextReadTime =
                            ImGui::GetTime();
                    }
                );


            if (!accepted)
            {
                m_reading = false;

                m_error =
                    "OpenFileAsync request rejected.";
            }
        }


        inline void UpdateSlowReader()
        {
            if (!m_reading)
                return;


            if (!m_readHandle)
                return;


            if (m_readRequestPending)
                return;


            if (m_readEOF)
                return;


            const double now =
                ImGui::GetTime();


            if (now < m_nextReadTime)
                return;


            QueueNextRead();
        }


        inline void QueueNextRead()
        {
            if (
                !m_readHandle ||
                m_readRequestPending ||
                m_readEOF
                )
                return;


            m_readRequestPending = true;


            const bool accepted =
                IO::ReadFileAsync(
                    m_readHandle,
                    m_readBuffer.data(),
                    m_readChunkSize,

                    [this](IO::Result& result)
                    {
                        m_readRequestPending =
                            false;


                        if (
                            result.status !=
                            IO::Status::COMPLETED
                            )
                        {
                            m_error =
                                "ReadFileAsync failed.";

                            StopSlowRead();

                            return;
                        }


                        const size_t read =
                            result.transferred_bytes;


                        m_totalReadBytes +=
                            read;


                        if (read > 0)
                        {
                            /*
                                Preview is text-oriented.

                                The IO itself can read arbitrary files;
                                binary files will simply display strange
                                characters in this demonstration panel.
                            */
                            m_readPreview.append(
                                m_readBuffer.data(),
                                read
                            );


                            /*
                                Avoid the UI preview growing forever.
                                Keep only the most recent ~64 KB.
                            */
                            constexpr size_t
                                MAX_PREVIEW =
                                64ull * 1024ull;


                            if (
                                m_readPreview.size() >
                                MAX_PREVIEW
                                )
                            {
                                m_readPreview.erase(
                                    0,
                                    m_readPreview.size() -
                                    MAX_PREVIEW
                                );
                            }
                        }


                        m_readEOF =
                            result.eof;


                        if (m_readEOF)
                        {
                            CloseSlowReadHandle();
                            return;
                        }


                        /*
                            Wait before issuing the next read.
                        */
                        m_nextReadTime =
                            ImGui::GetTime() +
                            static_cast<double>(
                                m_readIntervalSeconds
                                );
                    }
                );


            if (!accepted)
            {
                m_readRequestPending =
                    false;

                m_error =
                    "ReadFileAsync request rejected.";

                StopSlowRead();
            }
        }


        inline void StopSlowRead()
        {
            /*
                If a read is currently pending we cannot safely
                destroy/resize the read buffer.

                For this simple demo we mark the reader as stopping
                and close after the pending callback arrives.
            */
            if (m_readRequestPending)
            {
                m_stopReadRequested = true;
                return;
            }


            CloseSlowReadHandle();
        }


        inline void CloseSlowReadHandle()
        {
            if (!m_readHandle)
            {
                m_reading = false;
                m_stopReadRequested = false;
                return;
            }


            IO::Handle* handle =
                m_readHandle;

            m_readHandle = nullptr;


            const bool accepted =
                IO::CloseFileAsync(
                    handle,

                    [this](IO::Result& result)
                    {
                        m_reading = false;

                        m_readRequestPending =
                            false;

                        m_stopReadRequested =
                            false;


                        if (
                            result.status ==
                            IO::Status::COMPLETED
                            )
                        {
                            MS_INFO(
                                "Async reader closed file after %zu bytes",
                                m_totalReadBytes
                            );
                        }
                        else
                        {
                            MS_ERROR(
                                "Async reader failed to close file"
                            );
                        }
                    }
                );


            if (!accepted)
            {
                m_reading = false;
                m_stopReadRequested = false;
            }
        }


    private:

        // =====================================================
        // Directory state
        // =====================================================

        bool m_initialized = false;
        bool m_loading = false;

        std::string m_currentPath = ".";
        std::string m_error;

        std::vector<IO::DirectoryEntry>
            m_entries;

        int m_selectedIndex = -1;

        char m_pathBuffer[1024]{};


        // =====================================================
        // Heavy-file generator
        // =====================================================

        /*
            Exactly 1 GiB.
        */
        size_t m_heavyTargetSize =
            1024ull *
            1024ull *
            1024ull;

        size_t m_heavyBytesWritten = 0;

        bool m_creatingHeavyFile = false;

        IO::Handle* m_heavyHandle = nullptr;

        std::vector<char>
            m_writeBuffer;

        std::string m_heavyFilePath;

        std::string m_heavyStatus;

        char m_heavyFileName[256] =
            "heavy_test.bin";


        // =====================================================
        // Async slow reader
        // =====================================================

        bool m_reading = false;

        bool m_readRequestPending = false;

        bool m_readEOF = false;

        bool m_stopReadRequested = false;

        IO::Handle* m_readHandle =
            nullptr;

        std::string m_readFilePath;

        std::vector<char>
            m_readBuffer;

        std::string m_readPreview;

        size_t m_readChunkSize = 32;

        size_t m_totalReadBytes = 0;

        float m_readIntervalSeconds =
            1.0f;

        double m_nextReadTime = 0.0;
    };
	class ExampleSystem : public ISystem {
	public:

		virtual inline void OnAttach(void) noexcept override {
			MS_INFO("Example System On Attach");
            
		}

		virtual inline void OnDettach(void) noexcept override {
			MS_INFO("Example System On Detach");
		}

		virtual inline void OnUpdate(float deltaTime) noexcept override {
			if (Input::IsKeyPressed(MS_KEY_T)) {
				MS_INFO("Tab key is pressed !");
			}
		}

		virtual inline void OnEvent(Event& event) noexcept override {
			MultiStation::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MultiStation::KeyPressedEvent>(
				BIND_EVENT_FN(ExampleSystem::OnKeyPressedEvent));
		}

		inline bool OnKeyPressedEvent(MultiStation::KeyPressedEvent& e) {
			if (e.GetKeyCode() == MS_KEY_W) {
				MS_INFO("W key pressed :) ");
				
				return true;
			}
			return false;
		}
	};

	class ExampleModule : public ISystemModule {
	public:
		ExampleModule(void)  noexcept  {

		}

        virtual inline void Init(EngineContext& ctx) noexcept override
        {
            MS_INFO("Example Module Initialized");

            // =====================================================
            // Sync File Test
            // =====================================================

            {
                const std::string path = "./io_test.txt";
                const std::string text =
                    "MultiStation IO test\n"
                    "Second line\n";

                IO::Handle* handle =
                    IO::CreateFileSync(
                        path,
                        IO::Access::WRITE | IO::Access::READ
                    );

                if (!handle)
                {
                    MS_ERROR("CreateFileSync failed");
                }
                else
                {
                    MS_INFO(
                        "CreateFileSync success: %s",
                        path.c_str()
                    );

                    size_t written =
                        IO::WriteFileSync(
                            handle,
                            text.data(),
                            text.size()
                        );

                    MS_INFO(
                        "WriteFileSync wrote %zu bytes",
                        written
                    );

                    size_t pos =
                        IO::TellFileSync(handle);

                    MS_INFO(
                        "Position after write: %zu",
                        pos
                    );

                    IO::SeekFileSync(
                        handle,
                        0,
                        IO::SeekMode::START
                    );

                    char buffer[256]{};
                    bool dontcare;
                    size_t read =
                        IO::ReadFileSync(
                            handle,
                            buffer,
                            sizeof(buffer) - 1,
                            dontcare
                        );

                    buffer[read] = '\0';

                    MS_INFO(
                        "ReadFileSync read %zu bytes",
                        read
                    );

                    MS_INFO(
                        "File contents:\n%s",
                        buffer
                    );

                    if (IO::CloseFileSync(handle))
                    {
                        MS_INFO("CloseFileSync success");
                    }
                    else
                    {
                        MS_ERROR("CloseFileSync failed");
                    }
                }
            }


            // =====================================================
            // Sync Directory Test
            // =====================================================

            {
                const std::string dir = "./io_test_dir";

                if (IO::CreateDirSync(dir))
                {
                    MS_INFO(
                        "CreateDirSync success: %s",
                        dir.c_str()
                    );
                }
                else
                {
                    MS_ERROR(
                        "CreateDirSync failed: %s",
                        dir.c_str()
                    );
                }


                std::vector<IO::DirectoryEntry> entries;

                if (IO::ReadDirSync(".", entries))
                {
                    MS_INFO(
                        "ReadDirSync found %zu entries",
                        entries.size()
                    );

                    for (const auto& entry : entries)
                    {
                        MS_INFO(
                            "Name: %s | Path: %s | Directory: %d | Size: %zu",
                            entry.name.c_str(),
                            entry.path.c_str(),
                            static_cast<int>(entry.is_directory),
                            entry.size
                        );
                    }
                }
                else
                {
                    MS_ERROR("ReadDirSync failed");
                }


                if (IO::RemoveDirSync(dir))
                {
                    MS_INFO(
                        "RemoveDirSync success: %s",
                        dir.c_str()
                    );
                }
                else
                {
                    MS_ERROR(
                        "RemoveDirSync failed: %s",
                        dir.c_str()
                    );
                }
            }


            // =====================================================
            // Async File Test
            // =====================================================

            {
                IO::OpenFileAsync(
                    "./io_test.txt",
                    IO::Access::READ,
                    [](IO::Result& result)
                    {
                        if (result.status != IO::Status::COMPLETED)
                        {
                            MS_ERROR(
                                "OpenFileAsync failed. Error: %d",
                                static_cast<int>(result.error)
                            );

                            return;
                        }

                        MS_INFO(
                            "OpenFileAsync success. Handle: %p",
                            static_cast<void*>(result.handle)
                        );

                        IO::Handle* handle = result.handle;

                        if (!handle)
                            return;

                        static char buffer[256]{};

                        IO::ReadFileAsync(
                            handle,
                            buffer,
                            sizeof(buffer) - 1,
                            [handle](IO::Result& result)
                            {
                                if (
                                    result.status !=
                                    IO::Status::COMPLETED
                                    )
                                {
                                    MS_ERROR(
                                        "ReadFileAsync failed. Error: %d",
                                        static_cast<int>(result.error)
                                    );

                                    IO::CloseFileAsync(
                                        handle,
                                        [](IO::Result&) {}
                                    );

                                    return;
                                }

                                const size_t read =
                                    result.transferred_bytes;

                                buffer[read] = '\0';

                                MS_INFO(
                                    "ReadFileAsync read %zu bytes",
                                    read
                                );

                                MS_INFO(
                                    "Async contents:\n%s",
                                    buffer
                                );

                                IO::CloseFileAsync(
                                    handle,
                                    [](IO::Result& result)
                                    {
                                        if (
                                            result.status ==
                                            IO::Status::COMPLETED
                                            )
                                        {
                                            MS_INFO(
                                                "CloseFileAsync success"
                                            );
                                        }
                                        else
                                        {
                                            MS_ERROR(
                                                "CloseFileAsync failed. Error: %d",
                                                static_cast<int>(result.error)
                                            );
                                        }
                                    }
                                );
                            }
                        );
                    }
                );
            }


            Editor::AddDrawable(&m_panel);
        }


        virtual inline void Fini(
            EngineContext& ctx
        ) noexcept override
        {
            if (IO::RemoveFileSync("./io_test.txt"))
            {
                MS_INFO(
                    "Test file removed successfully"
                );
            }
            else
            {
                MS_ERROR(
                    "Failed to remove test file"
                );
            }

            MS_INFO(
                "Example Module Finilized"
            );
        }

		virtual inline ISystem& GetSystem(void) noexcept override {
			return m_system;
		}
		
		
	private:
		ExampleSystem m_system;
		ExamplePanel m_panel;

	};
}
