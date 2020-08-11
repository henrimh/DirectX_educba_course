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
	output.pos = pos;
	output.pos.xy *= 0.7; // shriking vertices on the x and y axis
	output.color = color;


	// return the struct
	return output;
}

// 1. Adding the desired variables to the shader
// 2. Create a C++ struct to match the variables in the shader
// 3. Create the constant buffer object. 
// 4. Set up the constant buffer object.
// 5. Update the contents of the constant using the struct, as needed.