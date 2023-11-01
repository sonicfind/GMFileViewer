#pragma once
#include "Graphics.h"
#include "FileContainer.h"
#include "GMException.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics_DX : public Graphics
{
public:
	class Exception : public GMException
	{
		using GMException::GMException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		const char* GetType() const noexcept override;

	private:
		std::string reason;
	};

	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};

private:
	template <typename T>
	class Shader_DX
	{
	public:
		const FileContainer& Data() { return file; }
		T* Get() const { return shader.Get(); }

	public:
		Shader_DX(const std::filesystem::path& path) : file(path) {}
		void Set(ID3D11Device& device);

	private:
		const FileContainer file;
		Microsoft::WRL::ComPtr<T> shader;
	};

	template <D3D11_USAGE USAGE, D3D11_BIND_FLAG BINDING, D3D11_CPU_ACCESS_FLAG CPU, int STRIDE>
	class Buffer_DX
	{
	public:
		static constexpr D3D11_BUFFER_DESC DESCRIPTION_base = {
			0,
			USAGE,
			BINDING,
			CPU,
			0,
			STRIDE
		};

	public:
		Buffer_DX(ID3D11Device& device, const D3D11_BUFFER_DESC& description, const void* data)
		{
			D3D11_SUBRESOURCE_DATA resource =
			{
				data, 0, 0
			};

			device.CreateBuffer(&description, &resource, &buffer);
		}

		ID3D11Buffer* Get() const { return buffer.Get(); }
		virtual void Delete() const { buffer->Release(); }

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	class VertexBuffer
		: public Buffer_DX<D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, 13 * sizeof(float)>
	{
	public:
		VertexBuffer(ID3D11Device& device, const D3D11_BUFFER_DESC& description, const void* data, const D3D11_INPUT_ELEMENT_DESC* inputDesc, const FileContainer& shader);
		void bind(ID3D11DeviceContext& context) const;
		virtual void Delete() const override;
	
	private:
		const UINT vertexSize;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
	};

	using IndexBuffer = Buffer_DX<D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, (D3D11_CPU_ACCESS_FLAG)0, 0>;
	using ConstantBuffer = Buffer_DX<D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0>;

public:
	Graphics_DX(HINSTANCE hInstance);

private:
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wPAram, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wPAram, LPARAM lParam);

public:

	// Inherited via Graphics
	virtual void activateShader(ShaderType type) const override;
	virtual void setShaderInt(const char* name, int value) const override;
	virtual size_t createVertexBuffer(ShaderType type, const void* data, uint32_t dataSize, bool isDynamic) override;
	virtual void setActiveVertexBuffer(size_t index) override;
	virtual void bindVertexBuffer(size_t index) const override;
	virtual void updateVertexBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual size_t createIndexBuffer(const void* data, uint32_t dataSize) override;
	virtual void bindIndexBuffer(size_t index) const override;
	virtual void createTexture(std::string_view name, const void* imagePtr, uint32_t width, uint32_t height) override;
	virtual void bindTexture(std::string_view name) const override;
	virtual void updateTexture(uint32_t locationX, uint32_t locationY, const void* imagePtr, uint32_t width, uint32_t height) const override;
	virtual void unbindTexture() const override;
	virtual void selectSkyTexture(std::string_view name) override;
	virtual void setActiveTexture(uint32_t slot) const override;
	virtual void bindConstantBuffer(ConstBufferSelection selection) const override;
	virtual void updateConstantBuffer(uint32_t offset, const void* data, uint32_t dataSize) const override;
	virtual void setFrontFace(FrontFace front) override;
	virtual void setCullFunc(Culling cull) override;
	virtual void enable(Enablable property) const override;
	virtual void disable(Enablable property) const override;
	virtual void setDepthFunc(DepthTest testParam) const override;
	virtual void setBlendFunc(Blending blend) const override;
	virtual void setClearColor(float r, float g, float b, float a) const override;
	virtual void setTopology(PrimitiveMode mode) const override;
	virtual void drawArrays(uint32_t index, uint32_t count) const override;
	virtual void drawElements(uint32_t count, size_t offset) const override;
	virtual uint32_t addIgnorableDepthTexture() override;
	virtual void copyDepthToIgnorable(uint32_t id) const override;
	virtual void setDepthlessDepthTextures(uint32_t id) const override;
	virtual void resetFrame() const override;
	virtual void displayFrame() const override;
	virtual void updateTitle(const std::string& str) const override;
	virtual bool shouldClose() const override;

public:
	int GetExitCode() const noexcept;

private:
	void UpdateBuffer(uint32_t offset, const void* data, uint32_t dataSize) const;
	void SetRasterizerState() const;

private:
	static constexpr const char* wndClassName = "GMClass";

	HWND hWnd;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

	Shader_DX<ID3D11VertexShader> modelVertex;
	Shader_DX<ID3D11PixelShader>  modelPixel;
	Shader_DX<ID3D11VertexShader>   shadowVertex;
	Shader_DX<ID3D11GeometryShader> shadowGeometry;
	Shader_DX<ID3D11PixelShader>    shadowPixel;
	Shader_DX<ID3D11VertexShader>   spriteVertex;
	Shader_DX<ID3D11GeometryShader> spriteGeometry;
	Shader_DX<ID3D11PixelShader>    spritePixel;
	Shader_DX<ID3D11VertexShader> boxVertex;
	Shader_DX<ID3D11PixelShader>  boxPixel;

	std::vector<VertexBuffer> vertexBuffers;
	std::vector<IndexBuffer> indexBuffers;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterStates[2][3];
	FrontFace frontFace = FrontFace::Countrclockwise;
	Culling culling = Culling::DISABLED;

	mutable float clearColor[4] = { 0,0,0,1 };
	ID3D11Resource* bufferToUpdate = nullptr;
	mutable int ecode = INT_MIN;
};

template<>
void Graphics_DX::Shader_DX<ID3D11VertexShader>::Set(ID3D11Device& device);

template<>
void Graphics_DX::Shader_DX<ID3D11GeometryShader>::Set(ID3D11Device& device);

template<>
void Graphics_DX::Shader_DX<ID3D11PixelShader>::Set(ID3D11Device& device);

#define GMWND_EXCEPT( hr ) Graphics_DX::HrException( __LINE__,__FILE__,(hr) )
#define GMWND_LAST_EXCEPT() Graphics_DX::HrException( __LINE__,__FILE__,GetLastError() )
#define GMWND_NOGFX_EXCEPT() Graphics_DX::NoGfxException( __LINE__,__FILE__ )
