# xnu++ reliability profile
schema = 1
mode = semi-independent
provider_policy = prefer-tested-provider
security_failover = never
service_failover = bounded
required = capability-registry,structured-diagnostics,transactional-updates
optional = checkpoints,resource-quotas,device-simulator,live-diagnostics
recovery = signed-offline
updates = atomic-rollback
