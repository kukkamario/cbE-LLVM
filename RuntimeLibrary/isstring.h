#ifndef ISSTRING_H
#define ISSTRING_H
#include "common.h"

/**
  * Implicit-shared string
  */
struct ISString {
	public:
		friend ISString operator +(const string &,const ISString&);

		/** Constructs a null ISString. */
		ISString() : data(0){ }

		/** Constructs an ISString with str data. */
		ISString(const char *str);

		/** Constructs an ISString with str data. */
		ISString(const string &str);

		/** Copy constructor */
		ISString(const ISString &str);

		ISString(const char c);

		/** Destructor */
		~ISString();

		/** Assignment operator */
		ISString & operator=(const ISString &o);

		/** Returns copy of string data. */
		string getStdString()const;

		/** Returns a reference to string data.
		  * This reference will be valid until all references to the underlying string are destroyed. */
		const string &getRef()const;

		/** Returns the number of characters in the string */
		size_t length() const;

		/** Returns true if string is empty, false otherwise. */
		bool empty() const;

		/** Equality operator */
		bool operator == (const ISString &o) const;

		/** Unequality operator */
		bool operator != (const ISString &o) const;

		/** Addition operator */
		ISString operator + (const ISString &o) const;

		/** Addition operator */
		ISString operator + (const string &o) const;

		/** Returns true if the left operand is considered greater than the right operand, otherwise returns false. */
		bool operator > (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than the right operand, otherwise returns false. */
		bool operator < (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than or equal to the right operand, otherwise returns false. */
		bool operator >= (const ISString &o) const;

		/** Returns true if the left operand is considered smaller than or equal to the right operand, otherwise returns false. */
		bool operator <= (const ISString &o) const;

		/** Returns a reference to the UTF-8 encoded string data.
		  * This reference will be valid until all references to the underlying string are destroyed. */
		const string &getUtf8Encoded() const;

		/** Converts the string to a integer. Will return 0 if conversion fails.*/
		int32_t toInt() const;

		/** Converts the string to a float. Will return 0 if conversion fails.*/
		float toFloat() const;

		/** Converts the string to a byte. Will return 0 if conversion fails.*/
		uint8_t toByte() const;

		/** Converts the string to a short. Will return 0 if conversion fails.*/
		uint16_t toShort() const;

		static ISString fromInt(int32_t i);
		static ISString fromFloat(float f);

		/** Get a pointer to ALLEGRO_PATH from the string inside */
		//ALLEGRO_PATH* getPath() const;

		/** Disables or enables string utf-8 encoding in getUtf8Encoded. By default encoding is enabled.*/
		void requireEncoding(bool t);

		bool isEncodingRequired() const;
		static void construct(ISString *s);
	private:
		/** Implicit-shared data*/
		struct SharedData {
				SharedData():str(),  utfStr(0), noNeedForEncoding(false), refCounter(1) {}
				SharedData(const char * const s):str(s), utfStr(0), noNeedForEncoding(false), refCounter(1) {}
				SharedData(const string &s):str(s), utfStr(0), noNeedForEncoding(false), refCounter(1) {}
				~SharedData();

				/** String data */
				string str;

				/** UTF-8 encoded string data. Pointer is null if string hasn't been encoded yet */
				string *utfStr;

				/** True if is know that str doesn't contain non-ASCII characters. */
				bool noNeedForEncoding;

				/** Reference counter */
				uint32_t refCounter;

				/** Increases reference counter */
				void increaseRefCount();

				/** Decreases reference counter. If refCounter reaches 0, SharedData will be destroyed. */
				void decreaseRefCount();
		};
		ISString(SharedData *d):data(d){if (data) data->increaseRefCount();}

		/** Implicit-shared data */
		SharedData *data;

		const static string nullStdString;
};

/** std::string and ISString addition operator */
ISString operator +(const string &,const ISString&);


inline void ISString::SharedData::increaseRefCount() {
	refCounter++;
}

inline void ISString::SharedData::decreaseRefCount() {
	if (--refCounter == 0) {
		delete this;
	}
}

inline ISString::SharedData::~SharedData() {
	if (utfStr != 0) {
		delete utfStr;
	}
}



#endif // ISSTRING_H
