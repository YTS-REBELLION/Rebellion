#pragma once
class CError {
public:
	CError();
	~CError();

public:
	void error_display(const char* msg, int err_no);
};