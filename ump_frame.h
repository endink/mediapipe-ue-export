#pragma once

#include "ump_object.h"
#include "opencv2/core/mat.hpp"

using UmpFrameBase = UmpObject<IUmpFrame>;

class UmpFrame : public UmpFrameBase
{
	friend class UmpPipeline;

protected:
	virtual ~UmpFrame() override { log_d(strf("~UmpFrame %p", this)); }

public:
	UmpFrame(UmpCustomDtor& dtor) : UmpFrameBase(dtor) { log_d(strf("+UmpFrame %p", this)); }

	virtual const void* GetData() const override { return _cvmat.data; }
	virtual EUmpPixelFormat GetFormat() const override { return _format; }
	virtual int GetPitch() const override { return _cvmat.step; }
	virtual int GetWidth() const override { return _cvmat.cols; }
	virtual int GetHeight() const override { return _cvmat.rows; }

	inline cv::Mat& GetMatrixRef() { return _cvmat; }

protected:
	cv::Mat _cvmat;
	EUmpPixelFormat _format = EUmpPixelFormat::Unknown;
};
