
cbuffer ConstantBuffer
{
	float4x4 MatFinal;
};

struct VOut
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};


// Vertex shader
VOut main( float4 pos : POSITION, float4 color : COLOR)
{
	// Create a VOut struct
	VOut output;

	// set the output values
	output.pos = mul(MatFinal, pos); // renders the final 2D vector on the screen
	output.color = color;
	
	// return the struct
	return output;
}

// 1. Adding the desired variables to the shader
// 2. Create a C++ struct to match the variables in the shader
// 3. Create the constant buffer object. 
// 4. Set up the constant buffer object.
// 5. Update the contents of the constant using the struct, as needed.