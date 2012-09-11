#if !defined(CETTY_SHIRO_UTIL_SECURITYUTILS_H)
#define CETTY_SHIRO_UTIL_SECURITYUTILS_H
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

namespace cetty { namespace shiro {
    class SecurityManager;
}}

namespace cetty {
namespace shiro {
namespace util {

using namespace cetty::shiro;
/**
 * Accesses the currently accessible {@code Subject} for the calling code depending on runtime environment.
 *
 * @since 0.2
 */
class SecurityUtils {
private:
    /**
     * ONLY used as a 'backup' in VM Singleton environments (that is, standalone environments), since the
     * ThreadContext should always be the primary source for Subject instances when possible.
     */
   static SecurityManager *securityManager;

public:
    SecurityUtils(){}

    /**
     * Sets a VM (static) singleton SecurityManager, specifically for transparent use in the
     * {@link #getSubject() getSubject()} implementation.
     * <p/>
     * <b>This method call exists mainly for framework development support.  Application developers should rarely,
     * if ever, need to call this method.</b>
     * <p/>
     * The Shiro development team prefers that SecurityManager instances are non-static application singletons
     * and <em>not</em> VM static singletons.  Application singletons that do not use static memory require some sort
     * of application configuration framework to maintain the application-wide SecurityManager instance for you
     * (for example, Spring or EJB3 environments) such that the object reference does not need to be static.
     * <p/>
     * In these environments, Shiro acquires Subject data based on the currently executing Thread via its own
     * framework integration code, and this is the preferred way to use Shiro.
     * <p/>
     * However in some environments, such as a standalone desktop application or Applets that do not use Spring or
     * EJB or similar config frameworks, a VM-singleton might make more sense (although the former is still preferred).
     * In these environments, setting the SecurityManager via this method will automatically enable the
     * {@link #getSubject() getSubject()} call to function with little configuration.
     * <p/>
     * For example, in these environments, this will work:
     * <pre>
     * DefaultSecurityManager securityManager = new {@link org.apache.shiro.mgt.DefaultSecurityManager DefaultSecurityManager}();
     * securityManager.setRealms( ... ); //one or more Realms
     * <b>SecurityUtils.setSecurityManager( securityManager );</b></pre>
     * <p/>
     * And then anywhere in the application code, the following call will return the application's Subject:
     * <pre>
     * Subject currentUser = SecurityUtils.getSubject();</pre>
     *
     * @param securityManager the securityManager instance to set as a VM static singleton.
     */
    static void setSecurityManager(SecurityManager *securityManager) {
        SecurityUtils::securityManager = securityManager;
    }

    /**
     * Returns the SecurityManager accessible to the calling code.
     * <p/>
     * This implementation favors acquiring a thread-bound {@code SecurityManager} if it can find one.  If one is
     * not available to the executing thread, it will attempt to use the static singleton if available (see the
     * {@link #setSecurityManager setSecurityManager} method for more on the static singleton).
     * <p/>
     * If neither the thread-local or static singleton instances are available, this method throws an
     * {@code UnavailableSecurityManagerException} to indicate an error - a SecurityManager should always be accessible
     * to calling code in an application. If it is not, it is likely due to a Shiro configuration problem.
     *
     * @return the SecurityManager accessible to the calling code.
     * @throws UnavailableSecurityManagerException
     *          if there is no {@code SecurityManager} instance available to the
     *          calling code, which typically indicates an invalid application configuration.
     */
    static SecurityManager *getSecurityManager(){ return securityManager; }
};
}
}
}
#endif //#if !defined(CETTY_SHIRO_SECURITYUTILS_H)