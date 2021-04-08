R"(
kernel void Naive( const global float *A, const global float *B, global float *C, 
                   const uint C_rw, const uint C_cl, const uint AB_com_sz)
{
  const int globalRow = get_global_id(0);
  const int globalCol = get_global_id(1);

  float res = 0.0;

  for (uint i = 0; i < AB_com_sz; ++i)
    res += A[globalRow * AB_com_sz + i] * B[i * AB_com_sz + globalCol];

  C[globalRow * C_rw + globalCol] = res;
}
)"