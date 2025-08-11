namespace Vox
{
	namespace Utils
	{
		template <typename T>
		class optional
		{
			private:
				bool _hasValue = false;
				T _value;

			public:
				void operator=(const T &newValue)
				{
					_value = newValue;
					_hasValue = true;
				}
				bool operator==(const optional<T> newValue)
				{
					return newValue._value == this->_value;
				}
				bool operator!=(const optional<T> newValue)
				{
					return newValue._value != this->_value;
				}
				bool hasValue()
				{
					return _hasValue;
				}
				T value()
				{
					return _value;
				}
		};
	} // namespace Utils
}
