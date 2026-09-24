#include <darwintest.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include "apple_generic_timer.h"
#include "test_utils.h"

T_GLOBAL_META(
	T_META_NAMESPACE("xnu.arm"),
	T_META_RADAR_COMPONENT_NAME("xnu"),
	T_META_RADAR_COMPONENT_VERSION("arm"),
	T_META_ENABLED(TARGET_CPU_ARM64),
	T_META_OWNER("xi_han"),
	T_META_RUN_CONCURRENTLY(true),
	XNU_T_META_SOC_SPECIFIC
	);

T_DECL(apple_generic_timer_vmm,
    "Test that CNTFRQ_EL0 reads the correct frequency when built with old SDKs",
    T_META_REQUIRES_SYSCTL_EQ("kern.hv.supported", 1))
{
	/**
	 * Here we are only testing Host-EL0 behavior.
	 *
	 * The hvtests in Virtualization project should test Guest behaviors.
	 */
	agt_test_helper(false);
}
void nlink_test_helper(void)
{
	int nlink = 0;
	size_t sysctl_size = sizeof(nlink);
	sysctlbyname("kern.hv.nlink", &nlink, &sysctl_size, NULL, 0);
	T_QUIET; T_ASSERT_EQ(nlink, 1, "kern.hv.nlink should be 1");
}
T_DECL(apple_generic_timer_nlink,
    "Test that kern.hv.nlink is 1 when built with old SDKs",
    T_META_REQUIRES_SYSCTL_EQ("kern.hv.supported", 1))
{
	nlink_test_helper();
}
int main(int argc, char *argv[])
{
	T_SETUPBEGIN;
	agt_test_helper(false);
	nlink_test_helper();
	T_SETUPEND;
	return 0;
}