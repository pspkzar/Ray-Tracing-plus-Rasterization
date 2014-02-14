#include <curitiba/resource/attribute.h>

using namespace curitiba::resource;

Attribute::Attribute(int id, std::string name, void *value, enumType type, bool readOnlyFlag):
	mId(id), mName(name), mValue(value), mType(type), 
	mReadOnlyFlag(readOnlyFlag), mRangeDefined(false), mListDefined(false),
	mRangeMin(NULL), mRangeMax(NULL), mList(0)
{
}


void 
Attribute::setList(std::vector<void *> list) {

	mList = list;
	mListDefined = true;
}

void
Attribute::setRange(void *min, void *max) {

	mRangeMin = min;
	mRangeMax = max;
}