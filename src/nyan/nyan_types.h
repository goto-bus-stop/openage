#ifndef _NYAN_NYAN_TYPES_H_
#define _NYAN_NYAN_TYPES_H_

#include <memory>
#include <string>

namespace nyan {

// forward declaration
struct NyanType;

struct NyanDatatype {
	virtual ~NyanDatatype() = default;

	virtual std::string to_string() const = 0;
};

struct NyanBool : public NyanDatatype {
	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get();
};

struct NyanInt : public NyanDatatype {
	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get();
};

struct NyanFloat : public NyanDatatype {
	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get();
};

struct NyanString : public NyanDatatype {
	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get();
};

struct NyanCustomType : public NyanDatatype {
	std::weak_ptr<NyanType> type;

	NyanCustomType(std::shared_ptr<NyanType> type);
	~NyanCustomType() = default;

	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get(std::shared_ptr<NyanType> type);
};

struct NyanSet : public NyanDatatype {
	std::weak_ptr<NyanType> type;

	NyanSet(std::shared_ptr<NyanType> type);
	~NyanSet() = default;

	virtual std::string to_string() const;

	static std::shared_ptr<NyanDatatype> get(std::shared_ptr<NyanType> type);
};

}

#endif
