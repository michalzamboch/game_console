# https://github.com/adafruit/Adafruit-ST7735-Library
# https://github.com/adafruit/Adafruit-GFX-Library
# https://github.com/adafruit/Adafruit_BusIO

$repos =
    "Adafruit-ST7735-Library",
    "Adafruit-GFX-Library",
    "Adafruit_BusIO"


function Check-Repo ($repo_name) {

    $repo_link = "https://github.com/adafruit/"
    $full_repo_link = $repo_link + $repo_name

    if ( Test-Path -Path $repo_name )
    {
        Write-Output ""
        Write-Output ("Pulling: " + $repo_name)
        git pull
    }
    else
    {
        Write-Output ""
        git clone $full_repo_link
    }
}

# --------------------------------------------------

cd ..\lib

foreach ($repo in $repos)
{
    Check-Repo($repo)
}
