R"(
kernel void trivial( const global float *lhs, const global float *rhs, global float *ans, 
                     const uint C_rw, const uint AB_com_sz, const uint C_cl )
{
  const int globalRow = get_global_id(0);
  const int globalCol = get_global_id(1);
  float res = 0.0;
  for (uint i = 0; i < AB_com_sz; ++i)
    res += lhs[globalRow * AB_com_sz + i] * rhs[i * C_cl + globalCol];
  ans[globalRow * C_cl + globalCol] = res;
}
)"