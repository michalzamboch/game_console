# https://github.com/adafruit/Adafruit-ST7735-Library
# https://github.com/adafruit/Adafruit-GFX-Library
# https://github.com/adafruit/Adafruit_BusIO

$repos =
    "adafruit/Adafruit-ST7735-Library",
    "adafruit/Adafruit-GFX-Library",
    "adafruit/Adafruit_BusIO"

function Check-Repo ($full_repo_name) {
    $full_repo_link = "https://github.com/" + $full_repo_name
    $short_repo_name = $full_repo_name.Replace("adafruit/", "")

    Write-Output ""
    if ( Test-Path -Path $short_repo_name )
    {
        Write-Output ("Pulling: " + $full_repo_name)
        pushd
        cd $short_repo_name
        git pull
        popd
    }
    else
    {
        git clone $full_repo_link
    }
}

# --------------------------------------------------

pushd
cd ..\lib
foreach ($repo in $repos)
{
    Check-Repo($repo)
}
popd
