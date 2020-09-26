#pragma once

#include <string>
#include "..\..\FileManegment\IFileCallback.h"
#include "IResource.h"

class FileListener;

namespace Graphics
{
	class IDeviceContext;

	class IShader : public IResource, public IFileCallback
	{
		friend class IPipelineState;

	public:
		virtual ~IShader() {}

		virtual bool Compile(const std::string& filename, const std::string& filepath, SHADER type) = 0;

		virtual const std::string& GetFilename() const = 0;
		virtual const std::string& GetFilepath() const = 0;
		virtual SHADER GetShaderType() const = 0;

	private:
		inline IResource* OnApiChange(API newApi) override
		{
			//TODO: Fix this function
			return nullptr;
		}

	public:
		static IShader* Create(const IDeviceContext* context, const std::string& filename, const std::string& filepath, 
			FileListener* const fileListener, SHADER type);
	};
}