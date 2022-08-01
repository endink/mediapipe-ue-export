#pragma once

#if !defined(PROF_ENABLE)

#include <string>

#define PROF_NAMED(name)
#define PROF_SCOPED
#define PROF_SUMMARY std::string()

#else

#include <cstdint>
#include <cstdio>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

#define PROF_STRINGIZE(x) PROF_STRINGIZE_INNER(x)
#define PROF_STRINGIZE_INNER(x) #x

#define PROF_CONCAT(a, b) PROF_CONCAT_INNER(a, b)
#define PROF_CONCAT_INNER(a, b) a ## b

#define PROF_UNIQ_VAR(base) PROF_CONCAT(base, __LINE__)
#define PROF_UNIQ_NAME __FILE__ ":" PROF_STRINGIZE(__LINE__)

#define PROF_NAMED(name) bicycle::ScopedProfiler PROF_UNIQ_VAR(prof_named_) (name, bicycle::file_name(PROF_UNIQ_NAME))
#define PROF_SCOPED bicycle::ScopedProfiler PROF_UNIQ_VAR(prof_scoped_) (__FUNCTION__, bicycle::file_name(PROF_UNIQ_NAME))
#define PROF_SUMMARY bicycle::Profiler::GetInstance().LogSummary()

namespace bicycle {

constexpr const char* file_name(const char* path) {
    const char* file = path;
    while (*path) {
        if (*path == '/' || *path == '\\') {
            file = path + 1;
        }
		path++;
    }
    return file;
}

class NonCopyable
{
private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
public:
	NonCopyable() {}
};

class ProfilerSection : public NonCopyable
{
public:
	ProfilerSection(const char* name, const char* uniq) :
		m_name(name), m_uniq(uniq), m_sum(0), m_count(0)
	{}

	void Begin() {
		m_ticks = std::chrono::high_resolution_clock::now();
	}

	void End() {
		const auto ticks = std::chrono::high_resolution_clock::now();
		const double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(ticks - m_ticks).count() * 1e-6; // milliseconds
		m_sum += delta;
		++m_count;
	}

	void ResetCounters() {
		m_sum = 0;
		m_count = 0;
	}

	const char* GetName() const { return m_name; }
	const char* GetUniq() const { return m_uniq; }
	size_t GetCount() const { return m_count; }
	double GetSum() const { return m_sum; }
	double GetAvg() const { return (m_count > 0 ? (m_sum / (double)m_count) : 0); }

private:
	const char* m_name;
	const char* m_uniq;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_ticks;
	double m_sum;
	size_t m_count;
};

class Profiler : public NonCopyable
{
public:
	typedef std::unordered_map<const char*, ProfilerSection*> SectionMap;

	Profiler() {}
	~Profiler() { Release(); }

	static Profiler& GetInstance() {
		static Profiler instance;
		return instance;
	}

	void Release() {
		auto l = std::lock_guard(m_lock);
		for (auto& iter : m_sections) {
			delete iter.second;
		}
		m_sections.clear();
	}

	ProfilerSection* GetSection(const char* name, const char* uniq) {
		auto l = std::lock_guard(m_lock);
		auto iter = m_sections.find(uniq);
		if (iter != m_sections.end()) {
			return iter->second;
		}
		else {
			auto* node = new ProfilerSection(name, uniq);
			m_sections.insert({ uniq, node });
			return node;
		}
	}

	std::string LogSummary() {
		auto l = std::lock_guard(m_lock);
		if (m_sections.empty()) {
			return std::string();
		}

		std::vector<ProfilerSection*> sections;
		sections.reserve(m_sections.size());

		for (auto& iter : m_sections) {
			sections.push_back(iter.second);
		}

		struct SectionComparatorSum {
			inline bool operator() (ProfilerSection* a, ProfilerSection* b) const { return a->GetSum() > b->GetSum(); }
		};
		struct SectionComparatorAvg {
			inline bool operator() (ProfilerSection* a, ProfilerSection* b) const { return a->GetAvg() > b->GetAvg(); }
		};
		std::sort(sections.begin(), sections.end(), SectionComparatorSum());

		constexpr int buf_size = 2048;
		char buffer[buf_size];

		std::string log;
		log.append("== PROFILER SUMMARY ==\n");

		snprintf(buffer, buf_size - 1,
			"%10s %10s %10s %10s  %-30s  %s\n",
			"<sum s>", "<sum ms>", "<calls>", "<avg ms>", "<name>", "<location>");
		log.append(buffer);

		for (auto* section : sections) {
			snprintf(buffer, buf_size - 1,
				"%10.3f %10.3f %10u %10.4f  %-30s  %s\n",
				(section->GetSum() * 0.001), section->GetSum(), (unsigned int)section->GetCount(), section->GetAvg(), section->GetName(), section->GetUniq());
			log.append(buffer);
		}

		log.append("\n");
		return std::move(log);
	}

	void Profiler::ResetCounters() {
		auto l = std::lock_guard(m_lock);
		for (auto& iter : m_sections) {
			iter.second->ResetCounters();
		}
	}

private:
	SectionMap m_sections;
	std::mutex m_lock;
};

class ScopedProfiler : public NonCopyable
{
public:
	ScopedProfiler(const char* name, const char* uniq) {
		m_section = Profiler::GetInstance().GetSection(name, uniq);
		m_section->Begin();
	}

	~ScopedProfiler() {
		m_section->End();
	}

private:
	ProfilerSection* m_section;
};

} // namespace

#endif
