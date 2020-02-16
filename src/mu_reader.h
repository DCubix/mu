#ifndef MU_READER_H
#define MU_READER_H

#include <string>
#include <vector>
#include <functional>

template <typename T>
class MuReader {
public:
	MuReader() = default;
	~MuReader() = default;

	inline bool eof() const { return m_buffer.empty(); }
	inline T read() { T val = m_buffer[0]; m_buffer.erase(m_buffer.begin()); return val; }
	inline T peek() const { return at(0); }
	inline T at(unsigned int i) const { return m_buffer[i]; }

	inline void setBuffer(const std::vector<T>& buf) { m_buffer = buf; }

private:
	std::vector<T> m_buffer;
};

class MuStringReader : public MuReader<char> {
public:
	MuStringReader() = default;
	MuStringReader(const std::string& data);

	std::string readWhile(const std::function<bool(char)>& func);

};

#endif // MU_READER_H
