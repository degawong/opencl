__kernel void hello_kernel(__global char16 *message){
	*message = (char16)('h', 'e', 'l', 'l', 'o', ' ', '_', 'w', 'o', 'r', 'l', 'd', '!', '!', '!', '\0');
}