
struct DPoint
{
	float X;
	float Y;
	DPoint()
	{
		X = 0.0f;
		Y = 0.0f;
	}
	DPoint(float _X, float _Y)
	{
		X = _X;
		Y = _Y;
	}
	void swap(DPoint&a, DPoint&b)
	{
		std::swap(a.X ,b.X);
		std::swap(a.Y ,b.Y);
	}
	DPoint operator=(DPoint a)
	{
		DPoint::swap(*this,a);
		return *this;
	}
};

bool operator<(DPoint const& a, DPoint const& b)
{
	if(a.X >= b.X)return false;
	if(a.Y >= b.Y)return false;
	return true;
}

bool operator>(DPoint const& a, DPoint const& b)
{
	if(a.X <= b.X)return false;
	if(a.Y <= b.Y)return false;
	return true;
}

bool operator==(DPoint const& a, DPoint const& b)
{
	if(a.X != b.X)return false;
	if(a.Y != b.Y)return false;
	return true;
}

bool operator==(DPoint const& a, float b)
{
	if(a.X != b)return false;
	if(a.Y != b)return false;
	return true;
}

bool operator==(float b,DPoint const& a)
{
	if(a.X != b)return false;
	if(a.Y != b)return false;
	return true;
}

DPoint operator-(DPoint const& a, DPoint const& b)
{
	return DPoint(a.X-b.X,a.Y-b.Y);
}

DPoint operator-(DPoint const& a, float b)
{
	return DPoint(a.X-b,a.Y-b);
}

DPoint operator-(float a, DPoint const& b)
{
	return DPoint(a-b.X,a-b.Y);
}

DPoint operator+(DPoint const& a, DPoint const& b)
{
	return DPoint(a.X+b.X,a.Y+b.Y);
}

DPoint operator+(DPoint const& a, float b)
{
	return DPoint(a.X+b,a.Y+b);
}

DPoint operator+(float a, DPoint const& b)
{
	return DPoint(a+b.X,a+b.Y);
}

DPoint operator/(DPoint const& a, DPoint const& b)
{
	return DPoint(a.X/b.X,a.Y/b.Y);
}

DPoint operator/(DPoint const& a, float b)
{
	return DPoint(a.X/b,a.Y/b);
}

DPoint operator/(float a, DPoint const& b)
{
	return DPoint(a/b.X,a/b.Y);
}

DPoint operator*(DPoint const& a, DPoint const& b)
{
	return DPoint(a.X*b.X,a.Y*b.Y);
}

DPoint operator*(DPoint const& a, float b)
{
	return DPoint(a.X*b,a.Y*b);
}

DPoint operator*(float a, DPoint const& b)
{
	return DPoint(a*b.X,a*b.Y);
}

DPoint function_u(DPoint input)
{
	return input / (sqrt((input.X * input.X) + (input.Y * input.Y))); // (1)
}

DPoint function_r(DPoint input)
{
	return function_u(DPoint(input.Y, -input.X)); // (2)
}
