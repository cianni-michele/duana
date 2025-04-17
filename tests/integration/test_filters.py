import subprocess

def test_filter_types(tmp_path):
    # Prepare files
    (tmp_path / "a.c").write_text("code");
    (tmp_path / "b.txt").write_text("text");
    (tmp_path / "c.log").write_text("log");

    # Run with filter .c and .txt
    result = subprocess.run([
        "../build/duana",
        str(tmp_path),
        "--types=.c,.txt"
    ], capture_output=True, text=True)

    assert result.returncode == 0
    output = result.stdout
    assert "Total files: 2" in output