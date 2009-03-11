/*
 * Copyright 2005,2009 WSO2, Inc. http://wso2.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCT_PROVIDER_H
#define SCT_PROVIDER_H

#include <WSFDefines.h>
#include <string>
#include <rampart_sct_provider.h>
#include <MessageContext.h>

/**
 * @file SCTProvider.h
 */

/**
 * @brief namespace wso2wsf Namespace of WSO2 Web Services Framework.
 */
namespace wso2wsf
{
    /**
     * @defgroup SCTProvider SCTProvider
     * @ingroup wso2wsf
     * @{
     */

    /**
     * @brief class SCTProvider Represents an interface needed implement any user specific
     * security context token provider functionality
     */
    class SCTProvider
    {
    public:
        /**
         * Handler to be invoked to get previously stored sct
         */
        virtual void* WSF_CALL getToken(
            std::string sct_id, 
            MessageContext* msgctx) = 0;

        /**
         * Handler to be invoked to store sct
         */
        virtual bool WSF_CALL storeToken(
            std::string sct_id, 
            MessageContext* msgctx, 
            void* sct) = 0;

        /**
         * Handler to be invoked to remove previously stored sct
         */
        virtual bool WSF_CALL removeToken(
            std::string sct_id, 
            MessageContext* msgctx) = 0;

        /**
         * Handler to be invoked to validate whether security context token is valid or not
         */
        virtual bool WSF_CALL validateToken(
            OMNode* sct_node, 
            MessageContext* msgctx) = 0;

        /**
         * destructor that can be overridden.
         */
        virtual WSF_CALL ~SCTProvider();

        /**
         * Static Handler to be invoked to store the sct
         */
        static axis2_status_t WSF_CALL storeToken(
            const axutil_env_t *env, 
            axis2_msg_ctx_t* msg_ctx, 
            axis2_char_t *sct_global_id, 
            axis2_char_t *sct_local_id, 
            void *sct, 
            void *user_params);

        /**
         * Static Handler to be invoked to get previously stored sct
         */
        static void* WSF_CALL getToken(
            const axutil_env_t *env, 
            axis2_bool_t is_encryption, 
            axis2_msg_ctx_t* msg_ctx, 
            axis2_char_t *sct_id, 
            int sct_id_type,
            void* user_params);

        /**
         * Static Handler to be invoked to remove previously stored sct
         */
        static axis2_status_t WSF_CALL removeToken(
            const axutil_env_t *env, 
            axis2_msg_ctx_t* msg_ctx, 
            axis2_char_t *sct_id, 
            int sct_id_type,
            void* user_params);
        
        /**
         * Static Handler to be invoked to validate whether security context token is valid or not
         */
        static axis2_status_t WSF_CALL validateToken(
            const axutil_env_t *env, 
            axiom_node_t *sct_node, 
            axis2_msg_ctx_t *msg_ctx, 
            void *user_params);
    };
    /** @} */
}

#define WSF_SCT_PROVIDER_INIT(class_name) \
extern "C" \
{ \
    WSF_EXTERN int \
    axis2_remove_instance( \
        rampart_sct_provider_t *inst, \
        const axutil_env_t *env) \
    { \
        if (inst) \
        { \
            AXIS2_FREE(env->allocator, inst->ops); \
            AXIS2_FREE(env->allocator, inst); \
        } \
        return AXIS2_SUCCESS; \
    } \
\
    void* WSF_CALL wsf_sct_get_user_params(const axutil_env_t *env) \
    { \
        return new class_name; \
    } \
\
    WSF_EXTERN int \
    axis2_get_instance( \
        rampart_sct_provider_t **inst, \
        const axutil_env_t *env) \
    { \
        rampart_sct_provider_t* sct_provider = NULL; \
        wso2wsf::Process* process = NULL; \
    \
        sct_provider = (rampart_sct_provider_t*)AXIS2_MALLOC(env->allocator, sizeof(rampart_sct_provider_t)); \
    \
        sct_provider->ops = (rampart_sct_provider_ops_t *)AXIS2_MALLOC(env->allocator, sizeof(rampart_sct_provider_ops_t)); \
        sct_provider->ops->obtain_security_context_token = wso2wsf::SCTProvider::getToken; \
        sct_provider->ops->store_security_context_token = wso2wsf::SCTProvider::storeToken; \
        sct_provider->ops->delete_security_context_token = wso2wsf::SCTProvider::removeToken; \
        sct_provider->ops->validate_security_context_token = wso2wsf::SCTProvider::validateToken; \
        sct_provider->ops->get_user_params = wsf_sct_get_user_params; \
        sct_provider->ops->free = axis2_remove_instance; \
    \
        *inst = sct_provider; \
        if (!(*inst)) \
        { \
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[wsf_rampart]Cannot initialize the SCT provider module"); \
            return AXIS2_FAILURE; \
        } \
\
        process = wso2wsf::Process::getInstance(); \
        process->setEnv(env); \
\
        return AXIS2_SUCCESS; \
    } \
} \


#endif // SCT_PROVIDER_H