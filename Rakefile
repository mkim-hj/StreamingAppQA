# Suppress warnings that global access to Rake DSL methods are deprecated
include Rake::DSL

task :test do
    BUILDS.each do |build|
        build.test()
    end

    # Ensure the simulators are started from a fresh state for UI tests. Must
    # be a full Simulator reset to ensure Keychain is reset. Kill any running
    # simulators first since it is a prerequsite of simctl erase. Use
    # AppleScript since killall/pkill exit to soon.
    sh("osascript -e 'tell app \"Simulator\" to quit'")
    sh("xcrun simctl erase all")

    sh("#{BUILD_TOOL} test #{BUILD_FLAGS_UI_TEST} | #{PRETTIFY}")
end

task :set_info_plist_bundle_version do
    check_build_id_env()

    BUILDS.each do |build|
        build.set_info_plist_bundle_version()
    end
end

task :archives do
    BUILDS.each do |build|
        build.archive()
    end
end

task :zip_archives do
    BUILDS.each do |build|
        build.zip_archive()
    end
end

task :create_ipas do
    BUILDS.each do |build|
        build.create_ipa()
    end
end

task :upload_products => [:zip_archives] do
    BUILDS.each do |build|
        build.aws_uploads().each do |aws_upload|
            aws_upload.upload()
        end
    end
end

task :upload_to_hockeyapp do
    BUILDS.each do |build|
        build.upload_to_hockeyapp()
    end
end

task :clean do
    clean()
end

task :mrproper do
    mrproper()
end

task :bootstrap do
    # calling `rake bootstrap skip-cache` will bypass carthage_cache fetch
    last_arg = ARGV.last
    task last_arg.to_sym do ; end
    skip_cache = last_arg == "skip-cache"

    check_carthage()

    # Prompt user if in develop mode before undeveloping
    eval(`arc paste P164`)

    carthage_bootstrap(skip_cache: skip_cache)
end

task :update do
    check_carthage()

    # Prompt user if in develop mode before undeveloping
    eval(`arc paste P164`)

    if ARGV.length > 1
        dependency = ARGV[1]
        task dependency.to_sym do ; end
    end

    sh("carthage update #{dependency} --platform iOS")
end

task :build do
    check_carthage()

    carthage_build()
end

task :run_faux_pas do
    # Disabled until Faux Pas supports Xcode 7
    # sh("fauxpas check #{APP_NAME}.xcodeproj #{FAUX_PAS_FLAGS}")
end

task :develop do
    check_carthage()

    # Create symlinks to parent directory repos.
    sh('arc paste P146 | bash')

    carthage_build()
end

task :undevelop do
    sh('arc paste P147 | bash')
end

task :diff => [
    :clean,
    :bootstrap,
    # :test,
    # :run_faux_pas,
]

task :ci => [
    :clean,
    :bootstrap,
    # :test,
    # :run_faux_pas,
    :set_info_plist_bundle_version,
    :archives,
    :create_ipas,
    :upload_products,
    :upload_to_hockeyapp,
]

private

APP_NAME = 'Oregon'

# Xcodebuild

DERIVED_DATA_DIR = "#{ENV['HOME']}/Library/Developer/Xcode/DerivedData"
BUILD_TOOL = 'xcodebuild'
TEST_SDK = 'iphonesimulator'
ARCHIVE_SDK = 'iphoneos'

BUILD_FLAGS_TEST =
    "-destination 'platform=iOS Simulator,name=iPhone 5,OS=latest' "\
    "-destination 'platform=iOS Simulator,name=iPhone 6,OS=latest' "\
    "-enableCodeCoverage YES "\
    # Force active arch only during test to speed up tests and fix
    # slather issue https://github.com/venmo/slather/issues/103
    "ONLY_ACTIVE_ARCH=YES "\
    "-sdk #{TEST_SDK}"

BUILD_FLAGS_ARCHIVE = "-sdk #{ARCHIVE_SDK}"

PRETTIFY = "xcpretty; exit ${PIPESTATUS[0]}"

COMMIT_SHA = `git rev-parse --short HEAD`.strip
LOG_FOR_COMMIT_SHA = `git log -1 --pretty="%s" #{COMMIT_SHA}`.strip

def clean()
    sh("rm -rf #{DERIVED_DATA_DIR}/*")
    sh("rm -rf Carthage/Build/*")
end

def mrproper()
    sh("rm -rf #{DERIVED_DATA_DIR}/*")
    sh("rm -rf Carthage/*")
end

EXPORT_OPTIONS_PLIST_PATH = "export_options.plist"

# Carthage

def carthage_checkout_flags()
    build_is_ci() ? "--no-use-binaries" : ""
end

CARTHAGE_BUILD_FLAGS =
    "--platform iOS "\
    "--verbose"

def carthage_build()
    sh("carthage build #{CARTHAGE_BUILD_FLAGS} | #{PRETTIFY}")
end

def carthage_bootstrap(skip_cache: true)
    # Perform checkout, optionally downloading binaries
    sh("carthage checkout #{carthage_checkout_flags()}")

    # Try to fetch Carthage cache from S3
    if !skip_cache
        # Import carthage_cache_fetch() method
        eval(`arc paste P230`)
        valid_cache = carthage_cache_fetch() == 0

        if valid_cache
            # If a valid Carthage cache was fetched, prune Carthage/Checkouts
            # keeping only what's required to build iOS-App
            Dir.open('Carthage/Checkouts').each do |filename|
                next if CARTHAGE_CACHE_CHECKOUT_REQUIRED_PATHS.concat([ '.', '..' ]).include? filename

                # Otherwise, delete folder
                sh("rm -rf 'Carthage/Checkouts/#{filename}'")
            end
        end
    end

    carthage_build()

    # Only publish Carthage cache if it was built from sources
    if carthage_checkout_flags().include? "--no-use-binaries"
        # Import carthage_cache_publish() method
        eval(`arc paste P231`)
        carthage_cache_publish()
    end
end

def check_carthage()
    abort('Error: carthage not found. Please install carthage using Homebrew.') unless sh('which carthage')

    current_version = `carthage version`.strip
    required_version = '0.15.2'
    abort("Error: invalid carthage version #{current_version}, please install #{required_version} using Homebrew.") unless current_version == required_version
end

# Faux Pas

FAUX_PAS_CONFIG_PATH = 'FauxPasConfig/main.fauxpas.json'
FAUX_PAS_FLAGS = "--configFile #{FAUX_PAS_CONFIG_PATH}"

# puck

PUCK_API_KEY = '48d1bd51f45e41ad898806c1148b185d'
PUCK_FLAGS =
    "-submit=auto "\
    "-upload=all "\
    "-force=true "\
    "-commit_sha=#{COMMIT_SHA} "\
    "-api_token=#{PUCK_API_KEY} "\
    "-notes=\"#{LOG_FOR_COMMIT_SHA}\" "

# Versioning

def get_plist_value(path: nil, key: nil)
    require 'plist'

    plist = Plist::parse_xml(path)
    return plist[key]
end

def set_plist_value(path: nil, key: nil, value: nil)
    require 'plist'

    plist = Plist::parse_xml(path)
    plist[key] = value

    File.open(path, 'w') { |file|
        file.write(plist.to_plist)
    }
end

def check_build_id_env()
    abort('You must have the BUILD_ID environment variable defined to continue') unless build_id()
end

def build_id()
    ENV['BUILD_ID']
end

def build_is_diff()
    ENV['DIFF_ID'] != nil
end

def build_is_ci()
    ENV['BUILD_TYPE'] == "ci"
end

def build_is_jenkins()
    ENV['TARGET_PHID'] != nil
end

HTTPS_UPLOAD_DOMAIN = 'https://s3-us-west-2.amazonaws.com/builds.automatic.co/ios/oregon'
S3_UPLOAD_DOMAIN = 's3://builds.automatic.co/ios/oregon'
S3_REGION='us-west-2'

class AWSUpload
    def initialize(local_path: nil, s3_url: nil, content_type: nil, s3_region: nil)
        @local_path = local_path
        @s3_url = s3_url
        @content_type = content_type
        @s3_region = s3_region
    end

    def aws_s3_cp_flags()
        flags =
            "--region #{@s3_region} "\
            "'#{@local_path}' "\
            "'#{@s3_url}'"

        if @content_type
            return "--content-type '#{@content_type}' " + flags
        else
            return flags
        end
    end

    def upload()
        sh("aws s3 cp #{aws_s3_cp_flags}")
    end
end

class Build
    def initialize(distribution_type: nil, scheme_name: nil, archive_path: nil, ipa_path: nil, plist_path: nil, hockeyapp_app_id: nil)
        @distribution_type = distribution_type
        @scheme_name = scheme_name
        @archive_path = archive_path
        @ipa_path = ipa_path
        @plist_path = plist_path

        @hockeyapp_app_id = hockeyapp_app_id

        @archive_path_zip = "#{@archive_path}.zip"
    end

    def test()
        # Speed up by only testing enterprise scheme in diff builds.
        return if @distribution_type != :enterprise && build_is_diff()

        sh("#{BUILD_TOOL} test #{BUILD_FLAGS_TEST} -scheme #{@scheme_name} | #{PRETTIFY}")
    end

    def set_info_plist_bundle_version()
        set_plist_value(path: @plist_path, key: 'CFBundleVersion', value: build_id())
    end

    def archive()
        sh("#{BUILD_TOOL} archive #{BUILD_FLAGS_ARCHIVE} -scheme #{@scheme_name} -archivePath #{@archive_path} | #{PRETTIFY}")
    end

    def zip_archive()
        sh("ditto -c --keepParent #{@archive_path} #{@archive_path_zip}")
    end

    def export_options_method_type()
        @distribution_type == :enterprise ? 'enterprise' : 'app-store'
    end

    def create_export_options(options_plist_path:nil)
        require 'plist'

        # See xcodebuild --help for list of options
        plist_contents = {
            'method' => export_options_method_type(),
            # Disable bitcode upload for now, some 3rd party frameworks
            # we use don't have it enabled and iTunes Connect reject our build.
            'uploadBitcode' => false,
            'uploadSymbols' => true
        }

        File.open(options_plist_path, 'w') { |file|
            file.write(plist_contents.to_plist)
        }
    end

    def create_ipa()
        # The --exportPath option takes a folder, the exported ipa filename is
        # hardcoded to the 'Name' value in the archive Info.plist.
        # Extract it so we can rename the file later.
        name = get_plist_value(path: "#{@archive_path}/Info.plist", key: 'Name')

        sh("rm -f #{name}.ipa")
        sh("rm -f #{@ipa_path}")
        sh("rm -f #{EXPORT_OPTIONS_PLIST_PATH}")

        create_export_options(options_plist_path: "#{EXPORT_OPTIONS_PLIST_PATH}")
        sh("xcodebuild " \
           "-exportArchive " \
           "-exportOptionsPlist #{EXPORT_OPTIONS_PLIST_PATH} "\
           "-archivePath #{@archive_path} "\
           "-exportPath .")

        # Rename the hardcoded ipa name from xcodebuild script
        sh("mv #{name}.ipa #{@ipa_path}")
    end

    def upload_to_hockeyapp()
        release_flags = @distribution_type == :enterprise ? "-download=true -notify=true" : ""
        sh("puck #{PUCK_FLAGS} #{release_flags} -app_id=#{@hockeyapp_app_id} #{@archive_path}")
    end

    def aws_uploads()
        path = @distribution_type == :enterprise ? "enterprise" : "store"

        return [
            AWSUpload.new(
                local_path: @ipa_path,
                s3_url: "#{S3_UPLOAD_DOMAIN}/#{path}/#{build_id()}.ipa",
                content_type: 'application/octet-stream',
                s3_region: S3_REGION
            ),
            AWSUpload.new(
                local_path: @archive_path_zip,
                s3_url: "#{S3_UPLOAD_DOMAIN}/#{path}/#{build_id()}.archive.zip",
                content_type: 'application/zip',
                s3_region: S3_REGION
            )
        ]
    end
end

BUILDS = [
    Build.new(
        distribution_type: :enterprise,
        scheme_name: "#{APP_NAME}-Enterprise",
        archive_path: "#{APP_NAME}-Enterprise.xcarchive",
        ipa_path: "#{APP_NAME}-Enterprise.ipa",
        plist_path: "#{APP_NAME}/Info.plist",
        hockeyapp_app_id: "b65299726ad6472c8c03f90295dc84cb"
    ),
    Build.new(
        distribution_type: :submission,
        scheme_name: "#{APP_NAME}",
        archive_path: "#{APP_NAME}-Store.xcarchive",
        ipa_path: "#{APP_NAME}-Store.ipa",
        plist_path: "#{APP_NAME}/Info.plist",
        hockeyapp_app_id: "9f81d665ba0745688800a8e69477525d"
    )
]
