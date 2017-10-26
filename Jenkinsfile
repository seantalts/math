def setupCC(Boolean failOnError = true) {
    errorStr = failOnError ? "-Werror " : ""
    "echo CC=${env.CXX} ${errorStr}> make/local"
}

def setup(Boolean failOnError = true) {
    sh """
        git clean -xffd
        ${setupCC(failOnError)}
    """
}

def mailBuildResults(String label, additionalEmails='') {
    emailext (
        subject: "[StanJenkins] ${label}: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]'",
        body: """${label}: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]': Check console output at ${env.BUILD_URL}""",
        recipientProviders: [[$class: 'RequesterRecipientProvider']],
        to: "${env.CHANGE_AUTHOR_EMAIL}, ${additionalEmails}"
    )
}

def runTests(String testPath) {
    sh "./runTests.py -j${env.PARALLEL} ${testPath} || echo ${testPath} failed"
}

def distributionTests() {
    unstash 'MathSetup'
    sh setupCC(false)
    //runTests("test/prob")
    sh 'git status'
    retry(2) { junit 'test/**/*.xml' }
}

def updateUpstream(String upstreamRepo) {
    if (env.BRANCH_NAME == 'develop') {
        node('master') {
            retry(3) {
                checkout([$class: 'GitSCM',
                        branches: [[name: '*/develop']],
                        doGenerateSubmoduleConfigurations: false,
                        extensions: [[$class: 'SubmoduleOption',
                                    disableSubmodules: false,
                                    parentCredentials: false,
                                    recursiveSubmodules: true,
                                    reference: '',
                                    trackingSubmodules: false]],
                        submoduleCfg: [],
                        userRemoteConfigs: [[url: "git@github.com:stan-dev/${upstreamRepo}.git",
                                           credentialsId: 'a630aebc-6861-4e69-b497-fd7f496ec46b'
                ]]])
            }
            sh """
                curl -O https://raw.githubusercontent.com/stan-dev/ci-scripts/master/jenkins/create-${upstreamRepo}-pull-request.sh
                sh create-${upstreamRepo}-pull-request.sh
            """
            deleteDir()
        }
    }
}

pipeline {
    agent none
    parameters {
        string(defaultValue: 'downstream tests', name: 'cmdstan_pr',
          description: 'PR to test CmdStan upstream against e.g. PR-630')
        string(defaultValue: 'downstream tests', name: 'stan_pr',
          description: 'PR to test Stan upstream against e.g. PR-630')
    }
    options { skipDefaultCheckout() }
    stages {
        stage('Linting & Doc checks') {
            agent any
            steps {
                script {
                    retry(3) { checkout scm }
                    setup(false)
                    stash 'MathSetup'
                    sh setupCC()
                    parallel(
                        CppLint: { sh "make cpplint" },
                        //dependencies: { sh 'make test-math-dependencies' } ,
                        //documentation: { sh 'make doxygen' },
                        failFast: true
                    )
                }
            }
            post { always { deleteDir() } }
        }
        stage('Tests') {
            failFast true
            parallel {
                //stage('Headers') {
                //    agent any
                //    steps { 
                //        unstash 'MathSetup'
                //        sh setupCC()
                //        sh "make -j${env.PARALLEL} test-headers"
                //    }
                //    post { always { deleteDir() } }
                //}
                //stage('Unit') {
                //    agent any
                //    steps {
                //        unstash 'MathSetup'
                //        sh setupCC()
                //        runTests("test/unit")
                //        retry(2) { junit 'test/**/*.xml' }
                //    }
                //    post { always { deleteDir() } }
                //}
                //stage('CmdStan Upstream Tests') {
                //    when { expression { env.BRANCH_NAME ==~ /PR-\d+/ } }
                //    steps {
                //        build(job: "CmdStan/${params.cmdstan_pr}",
                //                    parameters: [string(name: 'math_pr', value: env.BRANCH_NAME)])
                //    }
                //}
                //stage('Stan Upstream Tests') {
                //    when { expression { env.BRANCH_NAME ==~ /PR-\d+/ } }
                //    steps {
                //        build(job: "Stan/${params.stan_pr}",
                //                    parameters: [string(name: 'math_pr', value: env.BRANCH_NAME)])
                //    }
                //}
                stage('Distribution tests (No RowVectors)') {
                    agent { label "distribution-tests" }
                    steps { distributionTests() }
                    post { always { deleteDir() } }
                }
                stage('Full distribution tests') {
                    agent { label "distribution-tests" }
                    when { branch 'develop' }
                    steps {
                        sh "git revert -m1 997ae6e0a92128997ceb191901785e0827ffe000"
                        distributionTests()
                    }
                    post { always { deleteDir() } }
                }
            }
        }
    }
    post {
        always {
            node('master') {
                warnings consoleParsers: [[parserName: 'GNU C Compiler 4 (gcc)']], canRunOnFailed: true
                warnings consoleParsers: [[parserName: 'Clang (LLVM based)']], canRunOnFailed: true
                warnings consoleParsers: [[parserName: 'CppLint']], canRunOnFailed: true
                warnings consoleParsers: [[parserName: 'math-dependencies']], canRunOnFailed: true
            }
        }
        success {
            updateUpstream('stan')
            //mailBuildResults("SUCCESSFUL")
        }
        //unstable { mailBuildResults("UNSTABLE", "stan-buildbot@googlegroups.com") }
        //failure { mailBuildResults("FAILURE", "stan-buildbot@googlegroups.com") }
    }
}
