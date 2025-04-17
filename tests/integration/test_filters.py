import subprocess
from pathlib import Path

def test_filter_types(tmp_path):
    # Prepare files
    (tmp_path / "a.c").write_text("code");
    (tmp_path / "b.txt").write_text("text");
    (tmp_path / "c.log").write_text("log");

    # Determine path to the duana executable (two levels up to project root)
    project_root = Path(__file__).resolve().parents[2]
    duana_exe = project_root / "build" / "duana"

    # Run with filter .c and .txt
    result = subprocess.run(
        [str(duana_exe), str(tmp_path),"--types=.c,.txt"],
        capture_output=True, text=True
    )

    assert result.returncode == 0
    output = result.stdout
    assert "Total files: 2" in output